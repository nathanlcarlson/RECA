import numpy as np
import matplotlib.pyplot as plt
import argparse
def main():
    parser = argparse.ArgumentParser(description='')
    
    parser.add_argument('-c','--conf', dest='conffiles', nargs='+',
                        help='')

    args = parser.parse_args()
    # Number of times to sample a simulation
    n = 10
    # Time interval, lag
    w = 900
    # Time to start at
    start = w*10
    handles = [""]*n
    auto_correlations = []
    for simulation in args.conffiles:
        sim = np.genfromtxt(simulation)
        for m in range(len(handles)):
            t = np.random.random_integers(len(sim) - w - 1)
            auto_correlations.append(auto_correlation(sim, t, t+w))
            # auto_correlations.append(auto_correlation(sim, start + w*m, start + w*(m+1)))


            # handles[m], = plt.plot(  auto_correlations[m],
            #                          label = str(start + w*m) + "-" + str( start + w*(m+1))
            #                       )
    # n_bins = 30
    # for m in range(len(handles)):
    #     plt.plot( binned_avg(auto_correlations[m], n_bins) )

    plt.plot(overall_avg(auto_correlations))
    # for i in range(len(args.filenames)):
    #     plt.plot(np.genfromtxt(args.filenames[i]))
    #
    # plt.legend(handles=handles)
    plt.show()

def auto_correlation(configuration, t_start, t_stop):
    print t_start, t_stop
    conf = configuration[t_start:t_stop]
    x = np.cos(conf)
    y = np.sin(conf)
    l = len(x[0])
    n_samples = 100
    c_dt = np.zeros(len(conf) - n_samples - 1)
    n_dts = len(c_dt)
    for dt in range(n_dts):
        n = 0
        while n != n_samples:
            t = np.random.random_integers(len(conf) - dt - 1)
            c_dt[dt] += np.sum(x[t]*x[t+dt] + y[t]*y[t+dt])
            n+=1
        c_dt[dt] = c_dt[dt]/(l*n_samples)

    return c_dt

def binned_avg(data_t, n_bins):
    bin_width = int(len(data_t)/n_bins)
    out = np.zeros(n_bins-1)
    for b in range(n_bins-1):
        out[b] = np.mean( data_t[ b*bin_width : bin_width*(b+1) ] )

    return out

def overall_avg(data):

    return np.mean(data,axis=0)
main()
