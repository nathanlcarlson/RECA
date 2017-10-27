#!/usr/bin/python
import argparse
import os
def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-b','--beta', dest='betas', nargs='+',
                        help='Inverse temperature')
    parser.add_argument('-t','--steps', dest='steps', default = "100000",
                        help='Number of Monte Carlo steps in a simulation')
    parser.add_argument('-f','--freq', dest='freqs', nargs='+',
                        help='Percentage of RECA moves to use, 0 - 100')
    parser.add_argument('-L','--width', dest='width', nargs='+',
                        help='The width of the lattice to simulate')
    parser.add_argument('-n','--samples', dest='samples', type=int, default = 1,
                        help='Number of simulations to do for each unique parameter set')
    parser.add_argument('-r','--root', dest='root', default = "data",
                        help='The root directory to store data in')
    args = parser.parse_args()

    m_dict = {}
    for L in args.width:
      m_dict[L] = {}
      for beta in args.betas:
        m_dict[L][beta] = {}
        for freq in args.freqs:
          m_dict[L][beta][freq] = {}
          m_dict[L][beta][freq]['files'] = {}
          m_dict[L][beta][freq]['files']['energy'] = []
          m_dict[L][beta][freq]['files']['conf'] = []
          for c in range(args.samples):

            path = args.root+"/L"+L+"/beta"+beta+"/freq"+freq
            mkdirs(path)
            path = path+"/"+str(c)
            print path
            m_dict[L][beta][freq]['files']['conf'].append(path+".conf")
            m_dict[L][beta][freq]['files']['energy'].append(path+".enr")
            os.system("./reca " +
                       path + " " +
                       L + " " +
                       beta + " " +
                       freq + " " +
                       args.steps
                     )

          #os.system("python proc/energy.py -c "+ " ".join(controls) + " -e " + " ".join(expr))


def mkdirs(path):
  try:
    os.makedirs(path)
  except OSError:
    pass


main()
