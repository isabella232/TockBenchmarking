import csv
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats

memcpy = []
tests = {}
with open("proc.log") as csvfile:
    for line in csvfile:
        if 'benchmark' in line:
            test = line.split()[1].strip(', ').split('proc')[1].strip('_')
            test = "Procedure call benchmark, " + test
            size = line.split()[-1].split('proc')[0].strip('_')
            if test not in tests:
                tests[test] = {}
            if size not in tests[test]:
                tests[test][size] = []
            continue
        elif 'run' in line or 'RESULT' in line:
            continue
        data = [int(x.strip()) for x in line.split(',')][-1]
        tests[test][size].append(data)

for test in tests:
    for size in tests[test]:
        tests[test][size] = np.mean(tests[test][size])
    tests[test] = np.array(list(tests[test].items()))

    yest = 11.4 + 0.4 * np.arange(0,9)
    yest = np.concatenate(([11] ,yest, 14.6 + 0.5*(np.arange(9,11)-8)))


    label = 'time (µs)' if 'cycle' in test  else 'instructions'
    plt.title(test)
    plt.ylabel(label)
    plt.xlabel("size")
    plt.plot(tests[test][:,0], tests[test][:,1].astype(float)/10, label=test)
    filename = test.replace(' ', '_')
    filename = filename.replace(',', '')
    fit = np.polyfit(np.arange(0, tests[test].shape[0]), tests[test][:,1].astype(float)/10, 1)
    slope, intercept,  r, p, stderr = scipy.stats.linregress(np.arange(0, tests[test].shape[0]), tests[test][:,1].astype(float)/10)
    plt.plot(tests[test][:,0], fit[0]*np.arange(0, tests[test].shape[0]) + fit[1], label='y={:.3f}x+{:.3f}, R^2={:.3f}'.format(*fit, r**2))
    plt.legend()
    plt.savefig(filename + '.svg')
    plt.close()

    if 'cycle' in test:
        plt.title(test + ' estimated')
        plt.ylabel(label)
        plt.xlabel("size")
        plt.plot(tests[test][:,0], yest, label=test + ' estimated')
        fit = np.polyfit(np.arange(0, tests[test].shape[0]), yest, 1)
        slope, intercept,  r, p, stderr = scipy.stats.linregress(np.arange(0, tests[test].shape[0]), yest)
        plt.plot(tests[test][:,0], fit[0]*np.arange(0, tests[test].shape[0]) + fit[1], label='y={:.3f}x+{:.3f}, R^2={:.3f}'.format(*fit, r**2))
        plt.legend()
        plt.savefig(filename + '_estimated.svg')
        plt.close()

