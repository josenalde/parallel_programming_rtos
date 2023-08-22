import time
import os
import pandas as pd

tstart = time.time()

files = os.listdir()

for fp in files: #some directory with many excel files to open
    if 'xlsx' in fp:
        df = pd.read_excel(fp)
        total = df['valor_final'].sum()
        print(f'total of file {fp.replace('.xlsx','')} is {total,.2f}')
print(f'total time: {time.time() - tstart}')

############# parallel ###############

#!pip install joblib
from joblib import Parallel, delayed

tstart = time.time()
files = os.listdir()

def calc_total(fp):
    if 'xlsx' in fp:
        df = pd.read_excel(fp)
        total = df['valor_final'].sum()
        return f'total of file {fp.replace('.xlsx','')} is {total,.2f}'

result = Parallel(n_jobs=2)(delayed(calc_total)(fp) for fp in files)
print(result)
print(f'total time: {time.time() - tstart}')