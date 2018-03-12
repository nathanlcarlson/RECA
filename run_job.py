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

    # ydefaults = {'beta': 1,
    #             'width': 10,
    #             'percent_RECA': 5,
    #             'samples': 5,
    #             'root': 'data',
    #             'n_states': 8
    #             }
    #
    # yargs = set_defaults(yargs, ydefaults)

    for sim in yargs:
        sim = yargs[sim];
        print(sim)
        for c in range(sim['samples']):
            L = str(sim['width'])
            beta = str(sim['beta'])
            freq = str(sim['percent_RECA'])
            N = str(sim['n_states'])
            if 'seed' in sim.keys():
                seed = str(sim['seed'])
            else:
                seed = str(randint(0,pow(2,20)))

            mkdirs(sim['root']+"/N"+N+"/L"+L+"/Beta"+beta+"/Freq"+freq)
            path = sim['root']+"/N"+N+"/L"+L+"/Beta"+beta+"/Freq"+freq+"/"+str(c)

            print("./RECA",L,beta,freq,path,seed,N)
            subprocess.Popen(["./RECA",L,beta,freq,path,seed,N],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)

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
