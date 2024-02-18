import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import os

if not os.path.isdir('figures'):
    os.mkdir('figures')

if not os.path.isdir('data'):
    os.mkdir('data')

if not os.path.isdir('build'):
    print('building cmake')
    subprocess.run(['cmake', '-B', 'build', '-DCMAKE_BUILD_TYPE=Debug'])
print('compile programm')
subprocess.run(['cmake', '--build' ,'build'])
print('running c++ backend(1 part)')
subprocess.run(['build/main'])
print('process results')
df = pd.read_csv('data/linear_search.csv')

fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, df.time)

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Линейный поиск')
fig.savefig(r'figures/linear_search.png')

df = pd.read_csv('data/binary_search.csv')

fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, df.time)

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Бинарный поиск')
fig.savefig(r'figures/binary_search.png')

df = pd.read_csv('data/sum_search_1.csv')

fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, df.time)

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Поиск суммы O(n^2) поиск')
fig.savefig(r'figures/sum_search_1_search.png')

df = pd.read_csv('data/sum_search_2.csv')

fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, df.time)

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Поиск суммы O(n) поиск')
fig.savefig(r'figures/sum_search_2_search.png')
print('saving results')