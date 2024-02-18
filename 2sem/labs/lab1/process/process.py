import argparse
import json
import os

parser = argparse.ArgumentParser()

parser.add_argument('input_path')
parser.add_argument('output_path')
parser.add_argument('config_path')

args = parser.parse_args()

with open(args.config_path) as f:
    config = json.load(f)

with open(args.input_path) as f:
    tex_file = f.read()

for var_name in config:
    if config[var_name][2] == 0:
        tex_file = tex_file.replace(f'&{var_name}&', f'({config[var_name][0]} \\pm {config[var_name][1]})')
    else:
        tex_file = tex_file.replace(f'&{var_name}&', f'({config[var_name][0]} \\pm '
                                                    f'{config[var_name][1]}) \\cdot 10 ^ '
                                                    f'{{{config[var_name][2]}}}')

with open(args.output_path, 'w') as f:
    f.write(tex_file)
