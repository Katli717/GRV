#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import os
output_dir = 'output'
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

data = pd.read_csv('output/output_time.csv')
generators = data['Generator'].unique()

for generator in generators:
    subset = data[data['Generator'] == generator]
    
    plt.figure(figsize=(10, 6))
    plt.plot(subset['Size'], subset['Time(ms)'], marker='o', label=generator)
    plt.title(f'Зависимость размера от времени для {generator}')
    plt.xlabel('Размер')
    plt.ylabel('Время (мс)')
    plt.grid()
    plt.legend()
    plt.savefig(os.path.join(output_dir, f'graph_{generator}.png'))
    plt.close() 

plt.figure(figsize=(10, 6))

for generator in generators:
    subset = data[data['Generator'] == generator]
    plt.plot(subset['Size'], subset['Time(ms)'], marker='o', label=generator)

plt.title('Зависимость размера от времени для всех генераторов')
plt.xlabel('Размер')
plt.ylabel('Время (мс)')
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.savefig(os.path.join(output_dir, 'combined_graph.png'))
plt.close()

print(f'Графики сохранены в папку "{output_dir}".')
