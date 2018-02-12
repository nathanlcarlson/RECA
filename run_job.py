#!/usr/bin/python
import argparse
import os
import subprocess
import yaml
from random import randint

def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-f','--yaml', dest='yaml', required = True,
                        help='YAML config file')
    args = parser.parse_args()

    with open(args.yaml) as f:
      yargs = yaml.safe_load(f)

    ydefaults = {'beta': [1], 'width': [10], 'percent_RECA': [5], 'samples': 5, 'root': 'data', 'steps': pow(2, 15)}

    yargs = set_defaults(yargs, ydefaults)

    for L in yargs['width']:
      for beta in yargs['beta']:
        for freq in yargs['percent_RECA']:
          for c in range(yargs['samples']):

            L = str(L)
            beta = str(beta)
            freq = str(freq)

            mkdirs(yargs['root']+"/L"+L+"/Beta"+beta+"/Freq"+freq)
            path = yargs['root']+"/L"+L+"/Beta"+beta+"/Freq"+freq+"/"+str(c)

            subprocess.Popen(["./RECA",
                              L,
                              beta,
                              freq,
                              str(yargs['steps']),
                              path,
                              str(randint(0,pow(2,20)))
                              ],
                              stdout=subprocess.PIPE,
                              stderr=subprocess.STDOUT)

def set_defaults(yargs, ydefaults):

  for key in ydefaults.keys():
    if key not in yargs.keys():
      yargs[key] = ydefaults[key]

  return yargs

def mkdirs(path):
  try:
    os.makedirs(path)
  except OSError:
    pass

main()
