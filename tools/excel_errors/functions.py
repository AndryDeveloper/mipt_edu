import xlwings as xw
import formulas
import re
import numpy as np

pattern_values = re.compile(r'[A-Z]\d+')
formulas_blacklist = []


def numeric_diff(func, variable_idx, point, eps=1e-5):
    lambd = eps * point[variable_idx]
    point_up = [p + lambd if i == variable_idx else p for i, p in enumerate(point)]
    point_down = [p - lambd if i == variable_idx else p for i, p in enumerate(point)]
    return (func(*point_up) - func(*point_down)) / (2 * lambd)


@xw.func
def ERR(formula, *args):
    values, errors = [], []
    for i, v in enumerate(args):
        if i % 2 == 0:
            values.append(float(v))
        else:
            if v is None:
                errors.append(0.)
            else:
                errors.append(float(v))
    func = formulas.Parser().ast(formula)[1].compile()
    err2 = 0.
    for i, err in enumerate(errors):
        err2 += (numeric_diff(func, i, values) * err) ** 2
    return float(err2 ** 0.5)