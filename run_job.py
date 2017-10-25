import argparse
import os
def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-b','--beta', dest='betas', nargs='+',
                        help='')
    parser.add_argument('-t','--steps', dest='steps', default = "100000",
                        help='')
    parser.add_argument('-f','--freq', dest='freqs', nargs='+',
                        help='')
    parser.add_argument('-L','--width', dest='width', nargs='+',
                        help='')
    parser.add_argument('-n','--samples', dest='samples', type=int,
                        help='')
    parser.add_argument('-r','--root', dest='root', default = "data", nargs='+',
                        help='')
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
