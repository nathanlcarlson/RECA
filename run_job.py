#!/usr/bin/python
import argparse
import os
import subprocess
def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-b','--beta', dest='betas', nargs='+',
                        help='Inverse temperature')
    parser.add_argument('-t','--steps', dest='steps', default = str(pow(2,15)),
                        help='Number of Monte Carlo steps in a simulation')
    parser.add_argument('-f','--freq', dest='freqs', nargs='+',
                        help='Percentage of RECA moves to use, 0 - 100')
    parser.add_argument('-L','--width', dest='width', nargs='+',
                        help='The width of the lattice to simulate')
    parser.add_argument('-n','--samples', dest='samples', type=int, default = 1,
                        help='Number of simulations to do for each unique parameter set')

    args = parser.parse_args()

    m_dict = {}
    for L in args.width:
      m_dict[L] = {}
      for beta in args.betas:
        m_dict[L][beta] = {}
        for freq in args.freqs:
          m_dict[L][beta][freq] = {}
          m_dict[L][beta][freq]['process'] = []
          for c in range(args.samples):


            m_dict[L][beta][freq]['process'].append(subprocess.Popen(["./reca",
                                                                  L,
                                                                  beta,
                                                                  freq,
                                                                  args.steps
                                                                 ],
                                                                  stdout=subprocess.PIPE,
                                                                  stderr=subprocess.STDOUT))


def mkdirs(path):
  try:
    os.makedirs(path)
  except OSError:
    pass

main()
