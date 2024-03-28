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
subprocess.run(['build/main'])
print('process results')

df = pd.read_csv('data/dynamic.csv')
fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(np.log10(df.N), np.log10(df.time_A), label='add 1')
ax.plot(np.log10(df.N), np.log10(df.time_B), label='add 100')
ax.plot(np.log10(df.N), np.log10(df.time_C), label='mul 2')

ax.set_xlabel(r"log N")
ax.set_ylabel(r"log T")
ax.set_title(r'Сложность добавления количества N элементов')
ax.legend()
fig.savefig(r'figures/dynamic.png')

df = pd.read_csv('data/io_flav.csv')
fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(np.log10(df.N), np.log10(df.time_A), label='Массив со сдвигом')
ax.plot(np.log10(df.N), np.log10(df.time_B), label='Кольцевой буфер')

ax.set_xlabel(r"log N")
ax.set_ylabel(r"log T")
ax.set_title(r'Решение задачи Иосифа Флавия')
ax.legend()
fig.savefig(r'figures/io_flav.png')

print('completed')