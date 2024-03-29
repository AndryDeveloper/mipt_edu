import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import subprocess
import os

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

df1 = pd.read_csv('data/strategy_a_1.csv')
df2 = pd.read_csv('data/strategy_a_2.csv')

fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df1.N, df1.time, label='uninformal')
ax.plot(df2.N, df2.time, label='unformal')
ax.legend()

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Стратегия А')
fig.savefig(r'figures/strategy_a.png')

df1 = pd.read_csv('data/strategy_b_1.csv')
df2 = pd.read_csv('data/strategy_b_2.csv')

fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df1.N, df1.time, label='uninformal')
ax.plot(df2.N, df2.time, label='unformal')
ax.legend()

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Стратегия B')
fig.savefig(r'figures/strategy_b.png')

df1 = pd.read_csv('data/strategy_c_1.csv')
df2 = pd.read_csv('data/strategy_c_2.csv')

fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df1.N, df1.time, label='uninformal')
ax.plot(df2.N, df2.time, label='unformal')
ax.legend()

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Стратегия С')
fig.savefig(r'figures/strategy_с.png')
print('saving results')