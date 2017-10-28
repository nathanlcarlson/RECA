import numpy as np
# import matplotlib.pyplot as plt
import argparse
import pylab as plt
def main():
  parser = argparse.ArgumentParser(description='')
  parser.add_argument('-c','--control', dest='control', nargs='+',
                      help='', required = True)
  parser.add_argument('-e','--energy', dest='energy', nargs='+',
                      help='', required = True)
  parser.add_argument('-a','--auto', dest='auto', action = 'store_true',
                      help='')
  parser.add_argument('-i','--ignore', dest='ignore', type = int , default = 0,
                      help='Ignore last N lines')
  args = parser.parse_args()

  expr = []
  for sim in args.energy:
      expr.append( np.genfromtxt( sim, skip_footer = args.ignore ) )
  control = []
  for sim in args.control:
      control.append( np.genfromtxt( sim, skip_footer = args.ignore ) )

  auto_expr = []
  for data in expr:
      auto_expr.append( autocorrelate( data ) )
  auto_control = []
  for data in control:
      auto_control.append( autocorrelate( data ) )


  plot_all(expr, "Experimental", 1)
  plot_all(control, "Control")
  plt.legend( loc = 'upper right' )
  plt.show()

  plot_all(auto_expr, "Experimental", 2)
  plot_all(auto_control, "Control")
  plt.legend( loc = 'upper right' )
  plt.show()

def plot_all(dset, label):

  i = 0
  plt.subplot()
  for data in dset:
    plt.plot(data, label = label+" "+str(i))
    i+=1

def avg_energy(data):

  return np.mean(data, axis = 0)

def autocorrelate(data):

  data = np.mean(data) - data
  result = np.correlate(data, data, mode='full')
  return result[result.size/2:]

main()
