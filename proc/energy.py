import numpy as np
import matplotlib.pyplot as plt
import argparse
def main():
  parser = argparse.ArgumentParser(description='')
  parser.add_argument('-c','--control', dest='control', nargs='+',
                      help='', required = True)
  parser.add_argument('-e','--energy', dest='energy', nargs='+',
                      help='', required = True)
  
  args = parser.parse_args()
  
  control = avg_energy(args.control)
  expr = avg_energy(args.energy)
  
  controlhandles, = plt.plot(control, label = "Control")
  exprhandles, = plt.plot(expr, label = "Expiermental")
  plt.legend( [controlhandles, exprhandles], ["Control", "Experimental"] )
  plt.show()
  
def avg_energy(data_files):
  samples = []
  for sim in data_files:
    samples.append(np.genfromtxt(sim))
    
  out = np.mean(samples, axis=0)
  print out.shape
  return out
  
main()
