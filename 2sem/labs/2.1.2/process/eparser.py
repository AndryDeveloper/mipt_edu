import pandas as pd
import numpy as np


def get_mean_value(arr):
    if len(arr) != 1:
        mean = np.mean(arr)
        rand_error = 1 / np.sqrt(len(arr) - 1) * np.std(arr)
        return mean, rand_error
    else:
        return arr[0], 0


def get_full_error(errors):
    return np.sqrt(sum(np.array(errors) ** 2))


def parse(path):
    df = pd.read_excel(path)
    df.dropna(inplace=True, how='all', axis=1)

    chunks = {}
    chunk_columns = []
    for i, column in enumerate(df.columns):
        if 'Unnamed' not in column and chunk_columns:
            chunks[chunk_columns[0]] = df[chunk_columns].dropna(how='all', axis=0).values
            chunk_columns = []
        chunk_columns.append(column)
        if i == len(df.columns) - 1:
            chunks[chunk_columns[0]] = df[chunk_columns].dropna(how='all', axis=0).values
            chunk_columns = []

    variables = {}
    if 'Exp' in chunks:
        chunk_ = chunks['Exp']
        for i, name in enumerate(chunk_[0]):
            if name.lower() != 'name':
                arr = chunk_[2:, i]
                value, random_error = get_mean_value(arr[pd.notna(arr)])
                error = get_full_error([random_error, chunk_[1][i]])
                variables[name] = (value, error)

    tables = {}
    for key in chunks:
        if key != 'Exp':
            table = {}
            chunk_ = chunks[key]
            borders = [i for i, name in enumerate(chunk_[0]) if pd.notna(name) and name.lower() != 'name']
            for i, name in enumerate(chunk_[0]):
                if pd.notna(name) and name.lower() != 'name':
                    j = borders.index(i)
                    if j == len(borders) - 1:
                        values = chunk_[2:, i:]
                    else:
                        values = chunk_[2:, i:borders[j + 1]]
                    exp_error = chunk_[1][i]
                    errors = np.array([get_full_error([get_mean_value(_)[1], exp_error]) for _ in values])
                    values = np.array([get_mean_value(_)[0] for _ in values])
                    table[name] = {'values': values, 'errors': errors}
            tables[key] = table
    return variables, tables
