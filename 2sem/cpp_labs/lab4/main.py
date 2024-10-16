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

df = pd.read_csv('data/tree_A.csv')
fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, np.log10(df.insert_time_A / df.N), label='insert_time_ordered')
ax.plot(df.N, np.log10(df.find_time_A / df.N), label='find_time_ordered')
ax.plot(df.N, np.log10(df.remove_time_A / df.N), label='remove_time_ordered')

ax.plot(df.N, np.log10(df.insert_time_B / df.N), label='insert_time_unordered')
ax.plot(df.N, np.log10(df.find_time_B / df.N), label='find_time_unordered')
ax.plot(df.N, np.log10(df.remove_time_B / df.N), label='remove_time_unordered')

ax.set_xlabel(r"N")
ax.set_ylabel(r"log T")
ax.set_title(r'Амортизационная проверка операций на множестве на базе несбалансированного дерева')
ax.legend()
fig.savefig(r'figures/tree_A.png')

df = pd.read_csv('data/tree_B.csv')
fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, np.log10(df.insert_time_A / df.N), label='insert_time_ordered')
ax.plot(df.N, np.log10(df.find_time_A / df.N), label='find_time_ordered')
ax.plot(df.N, np.log10(df.remove_time_A / df.N), label='remove_time_ordered')

ax.plot(df.N, np.log10(df.insert_time_B / df.N), label='insert_time_unordered')
ax.plot(df.N, np.log10(df.find_time_B / df.N), label='find_time_unordered')
ax.plot(df.N, np.log10(df.remove_time_B / df.N), label='remove_time_unordered')

ax.set_xlabel(r"N")
ax.set_ylabel(r"log T")
ax.set_title(r'Амортизационная проверка операций на множестве на базе АВЛ дерева')
ax.legend()
fig.savefig(r'figures/tree_B.png')

df = pd.read_csv('data/hashset.csv')
fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, np.log10(df.insert_time / df.N), label='insert_time')
ax.plot(df.N, np.log10(df.find_time / df.N), label='find_time')
ax.plot(df.N, np.log10(df.remove_time / df.N), label='remove_time')

ax.set_xlabel(r"N")
ax.set_ylabel(r"log T")
ax.set_title(r'Амортизационная проверка операций на множестве на базе хештаблицы')
ax.legend()
fig.savefig(r'figures/hashtable.png')

print('completed')