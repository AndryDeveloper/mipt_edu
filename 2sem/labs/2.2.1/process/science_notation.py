from math import floor, log10
from decimal import Decimal


def science_notation(x, err):
    if err == 0:
        return x, 0, 0
    x_mantissa, x_exp = get_man_exp(x)
    err_mantissa, err_exp = get_man_exp(err)
    err_norm = f"{err * 10 ** -x_exp:.{-err_exp + x_exp + 2}f}"
    err_round = round2one(err_norm)
    x_mantissa_dec = Decimal(x_mantissa).quantize(Decimal(err_round))
    return str(x_mantissa_dec), err_round, x_exp


def get_man_exp(x):
    e = floor(log10(abs(x)))
    m = x * 10 ** (-e)
    return m, e


def round2one(err):
    err = [0] + [int(_) for _ in err if _ != '.']
    last_item = err.pop(-1)
    if err[-2] == 1:
        if last_item != 0 and err[-1] == 9:
            err[-2] += 1
            err[-1] = 0
        elif last_item != 0:
            err[-2] += 1
    elif err[-2] == 2:
        if last_item != 0 and err[-1] == 9:
            err.pop(-1)
            err[-2] += 1
        elif last_item != 0:
            err[-2] += 1
    else:
        last_item = err.pop(-1)
        if err[-1] == 9 and last_item != 0:
            err[-2] += 1
            err[-1] = 0
        elif last_item != 0:
            err[-1] += 1

    err_str = ''
    fl = False
    for i, num in enumerate(err):
        if i == 2:
            if not err_str:
                err_str += '0'
            err_str += '.'
            fl = True
        if fl:
            err_str += str(num)
        elif num != 0:
            err_str += str(num)
            fl = True
    return err_str