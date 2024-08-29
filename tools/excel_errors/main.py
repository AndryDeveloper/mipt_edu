import xlwings as xw
import re

pattern_values = re.compile(r'[A-Z]\d+')


def main(path='from_excel'):
    if path == 'from_excel':
        wb = xw.Book.caller()
    else:
        wb = xw.Book(path)

    try:
        sheet_e = wb.sheets.add('Errors')
    except ValueError:
        sheet_e = wb.sheets['Errors']
    try:
        sheet_v = wb.sheets.add('Values')
    except ValueError:
        sheet_v = wb.sheets['Values']

    shape = sheet_v.used_range.shape
    for i in range(shape[0]):
        for j in range(shape[1]):
            curr_cell_v = sheet_v[(i, j)]
            if curr_cell_v.value is None:
                continue
            try:
                float(curr_cell_v.value)
            except ValueError:
                sheet_e[(i, j)].value = curr_cell_v.value
                continue
            if curr_cell_v.formula[0] != '=':
                continue

            values_lst = re.findall(pattern_values, curr_cell_v.formula)
            result = []
            for v in values_lst:
                result.append(f'Values!{v}')
                result.append(f'Errors!{v}')
            sheet_e[(i, j)].formula = '=ERR("{}",{})'.format(curr_cell_v.formula, ','.join(result))


if __name__ == '__main__':
    main('main.xlsm')
