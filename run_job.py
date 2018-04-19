#!/usr/bin/python
import argparse
import os
import subprocess
import yaml
import json
from random import randint

def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-f','--config', dest='config', required = True,
                        help='JSON config file')
    args = parser.parse_args()

    with open(args.config) as f:
      config = json.load(f)

    for sim in config:
        sim = config[sim];
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
            if 'cpu_seconds' in sim.keys():
                time = str(sim['cpu_seconds'])
            else:
                time = 30
            mkdirs(sim['root']+"/N"+N+"/L"+L+"/Beta"+beta+"/Freq"+freq)
            path = sim['root']+"/N"+N+"/L"+L+"/Beta"+beta+"/Freq"+freq+"/"+str(c)

            print(" ".join(["./RECA",L,beta,freq,path,seed,N,time]))
            subprocess.Popen(["./RECA",L,beta,freq,path,seed,N,time],stdout=subprocess.PIPE,stderr=subprocess.STDOUT)

def mkdirs(path):
  try:
    os.makedirs(path)
  except OSError:
    pass

main()
