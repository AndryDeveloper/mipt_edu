import torch
from eparser import parse
from typing import Tuple
import pandas as pd
from science_notation import science_notation
import json
from scipy.optimize import curve_fit
import numpy as np


class Table:
    def __init__(self, data, all_values, all_errors):
        dict_data = {}
        for column_name in data:
            x = torch.tensor(data[column_name]['values'], requires_grad=True, dtype=torch.float64)
            all_values.append(x)
            all_errors.append(data[column_name]['errors'])

            setattr(self, column_name, x)
            setattr(self, 'error_' + column_name, data[column_name]['errors'])

            dict_data[column_name] = [str(_) for _ in data[column_name]['values']]
            dict_data['error_' + column_name] = [str(_) for _ in data[column_name]['errors']]
        self.data = pd.DataFrame.from_dict(dict_data)

    def __repr__(self):
        return self.data.__repr__()
    def to_latex(self):
        return self.data.style.to_latex()


class Environment:
    def __init__(self, path: str, config_path='config.json'):
        """
        :param path: path to .xlsx file
        """
        self.__all_values = []
        self.__all_errors = []
        self.config = {}
        self.config_path = config_path

        variables, tables = parse(path)
        for var_name in variables:
            x = torch.tensor(variables[var_name][0], requires_grad=True, dtype=torch.float64)
            self.__all_values.append(x)
            self.__all_errors.append(variables[var_name][1])
            setattr(self, var_name, x)
            setattr(self, 'error_' + var_name, variables[var_name][1])

            self.config[var_name] = science_notation(variables[var_name][0], variables[var_name][1])

        for table_name in tables:
            table = Table(tables[table_name], self.__all_values, self.__all_errors)
            setattr(self, table_name, table)

        with open(self.config_path, 'w') as f:
            json.dump(self.config, f)

    def get_error(self, x):
        """
        :param x: x
        :return: error of x
        """
        if type(x) is list:
            full_errors = []
            for j in range(len(x)):
                grads = torch.autograd.grad(x[j], self.__all_values, retain_graph=True, allow_unused=True)
                full_error = torch.tensor(0., dtype=torch.float64)
                for i, error in enumerate(self.__all_errors):
                    if grads[i] is not None:
                        if grads[i].size():
                            full_error += sum((grads[i] * error) ** 2)
                        else:
                            full_error += (grads[i] * error) ** 2
                full_error **= 0.5
                full_errors.append(full_error.item())
            return torch.tensor(full_errors)
        elif not x.size():
            grads = torch.autograd.grad(x, self.__all_values, retain_graph=True, allow_unused=True)
            full_error = torch.tensor(0., dtype=torch.float64)
            for i, error in enumerate(self.__all_errors):
                if grads[i] is not None:
                    full_error += (grads[i] * error) ** 2
            return (full_error ** 0.5).detach()
        else:
            full_errors = []
            for j in range(len(x)):
                grads = torch.autograd.grad(x[j], self.__all_values, retain_graph=True, allow_unused=True)
                full_error = torch.tensor(0., dtype=torch.float64)
                for i, error in enumerate(self.__all_errors):
                    if grads[i] is not None:
                        if grads[i].size():
                            full_error += sum((grads[i] * error) ** 2)
                        else:
                            full_error += (grads[i] * error) ** 2
                full_error **= 0.5
                full_errors.append(full_error.item())
            return torch.tensor(full_errors)

    def chi2(self, x: torch.Tensor, y: torch.Tensor, params_name: dict, mode='classic', auto_reverse=True, reverse_axis=None):
        """
        Find a,b if y = a + bx
        :param x: x-tensor
        :param y: y-tensor
        :param params_name: {a: name_a, b: name_b}
        :param auto_reverse: reverse axis if y error less than x error
        :param mode: 'classic' if f =  a + bx, 'zero' if f = bx.
        :return: params a, b (a, b) if 'classic' mode else (0, b).
        """
        errors_x, errors_y = self.get_error(x).numpy(), self.get_error(y).numpy()
        x, y = torch.tensor(x).numpy(), torch.tensor(y).numpy()
        if reverse_axis is None:
            reverse_axis = (np.mean(errors_x / x) > np.mean(errors_y / y)) and auto_reverse
        if reverse_axis:
            print('Using reversed axis')
            x, y = y, x
            errors_x, errors_y = errors_y, errors_x
        
        if mode == 'classic':
            function = lambda x, a, b: a + b*x
            popt, pcov = curve_fit(function, xdata = x, ydata = y, sigma = errors_y)
            a, b = popt
            error_a = np.sqrt(pcov[0,0])
            error_b = np.sqrt(pcov[1,1])
        else:
            function = lambda x, b: b*x
            popt, pcov = curve_fit(function, xdata = x, ydata = y, sigma = errors_y)
            b = popt[0]
            a = 0
            error_a = 0
            error_b = np.sqrt(pcov[0,0])

        if reverse_axis:
            error_b, error_a = error_b / b ** 2, (((a * error_b) / b ** 2) ** 2 + (error_a / b) ** 2) ** 0.5
            b, a = 1 / b, -a / b

        a = torch.tensor(a, requires_grad=True, dtype=torch.float64)
        self.__all_values.append(a)
        self.__all_errors.append(error_a)

        b = torch.tensor(b, requires_grad=True, dtype=torch.float64)
        self.__all_values.append(b)
        self.__all_errors.append(error_b)

        setattr(self, params_name['a'], a)
        setattr(self, params_name['b'], b)
        
        self.config[params_name['a']] = science_notation(a.detach().item(), error_a)
        self.config[params_name['b']] = science_notation(b.detach().item(), error_b)

        return a, b

    def get_err_bars(self, x: torch.Tensor, y: torch.Tensor) -> Tuple[list, list]:
        """
        :param x: x-tensor
        :param y: y-tensor
        :return: (x_errors, y_errors)
        """
        return self.get_error(x).detach().numpy(), self.get_error(y).detach().numpy()

    def show(self, x, name=None):
        """
        :param x: x
        :param name: name of x (if you want to save it)
        :return: x and error of x in scientific form
        """
        if isinstance(x, Table):
            return str(x)
        elif not x.size():
            p = science_notation(x.detach().item(), self.get_error(x).detach().item())
            if name is not None:
                self.config[name] = p
                with open(self.config_path, 'w') as f:
                    json.dump(self.config, f)
            return p
        else:
            return [science_notation(_.detach().item(), self.get_error(_).detach().item()) for _ in x]