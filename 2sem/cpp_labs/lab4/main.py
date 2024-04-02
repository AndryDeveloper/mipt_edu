import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import subprocess
import os
from time import sleep

matplotlib.use('Agg')

if not os.path.isdir('figures'):
    os.mkdir('figures')

if not os.path.isdir('data'):
    os.mkdir('data')

if not os.path.isdir('build'):
    print('building cmake')
    subprocess.run(['cmake', '-B', 'build', '-DCMAKE_BUILD_TYPE=Debug'])
print('compile programm')
subprocess.run(['cmake', '--build' ,'build'])
print('running c++ backend')
# subprocess.run(['build/main'])
print('process results')

df = pd.read_csv('data/tree_A.csv')
fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, np.log10(df.insert_time_A), label='insert_time_ordered')
ax.plot(df.N, np.log10(df.find_time_A), label='find_time_ordered')
ax.plot(df.N, np.log10(df.remove_time_A), label='remove_time_ordered')

ax.plot(df.N, np.log10(df.insert_time_B), label='insert_time_unordered')
ax.plot(df.N, np.log10(df.find_time_B), label='find_time_unordered')
ax.plot(df.N, np.log10(df.remove_time_B), label='remove_time_unordered')

ax.set_xlabel(r"N")
ax.set_ylabel(r"log T")
ax.set_title(r'Проверка операций на множестве на базе несбалансированного дерева')
ax.legend()
fig.savefig(r'figures/tree_A.png')

print('completed')