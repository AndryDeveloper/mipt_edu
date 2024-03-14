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
df = pd.read_csv('data/shaker.csv')
fig, ax = plt.subplots(figsize=(14, 10))
ax.plot(df.N, df.time_full, label='shaker')
ax.plot(df.N, df.time_forward, label='forward')
ax.plot(df.N, df.time_backward, label='backward')

ax.set_xlabel(r"N")
ax.set_ylabel(r"Time")
ax.set_title(r'Шейкерная сортировка')
ax.legend()
fig.savefig(r'figures/shaker.png')

df = pd.read_csv('data/comb.csv')
fig, axs = plt.subplots(1, 2, figsize=(14, 5))

axs[0].plot(df.N, df.time)
axs[0].set_xlabel(r"N")
axs[0].set_ylabel(r"Time")
axs[0].set_title(r'Сортировка расчёской, время выполнения')

axs[1].plot(df.N, df.iters)
axs[1].set_xlabel(r"N")
axs[1].set_ylabel(r"Iterations")
axs[1].set_title(r'Сортировка расчёской, количество итераций')

fig.savefig(r'figures/comb.png')

df = pd.read_csv('data/shell.csv')
fig, axs = plt.subplots(1, 2, figsize=(14, 5))

axs[0].plot(df.N, df.time1, label='Последовательность Шелла')
axs[0].plot(df.N, df.time2, label='Последовательность Хиббарда')
axs[0].plot(df.N, df.time3, label='Последовательность Фиббоначи')
axs[0].set_xlabel(r"N")
axs[0].set_ylabel(r"Time")
axs[0].set_title(r'Сортировка Шелла, время выполнения')
axs[0].legend()

axs[1].plot(df.N, df.iters1, label='Последовательность Шелла')
axs[1].plot(df.N, df.iters2, label='Последовательность Хиббарда')
axs[1].plot(df.N, df.iters3, label='Последовательность Фиббоначи')
axs[1].set_xlabel(r"N")
axs[1].set_ylabel(r"Iterations")
axs[1].set_title(r'Сортировка Шелла, количество итераций')
axs[1].legend()

fig.savefig(r'figures/shell.png')
print('completed')