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
    
    for L in args.width:
      for beta in args.betas:
        for freq in args.freqs:
          for c in range(args.samples):
            path = args.root+"/L"+L+"/beta"+beta+"/freq"+freq
            print path+"/"+str(c)
            mkdirs(path)
            os.system("./reca " + 
                       path+"/"+str(c) + " " +
                       L + " " +
                       beta + " " +
                       freq + " " +
                       args.steps
                     )
        
def mkdirs(path):
  try:
    os.makedirs(path)
  except OSError:
    pass
    
                          
main()
