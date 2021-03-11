import csv
import numpy as np
import matplotlib.pyplot as plt

memcpy = []
tests = {}
with open("memory_bandwidth.log") as csvfile:
    for line in csvfile:
        if 'benchmark' in line:
            test = line.split()[1].strip(', ')
            size = int(line.split()[-1])
            if test not in tests:
                tests[test] = {}
            if size not in tests[test]:
                tests[test][size] = []
            continue
        elif 'run' in line:
            continue
        data = [int(x.strip()) for x in line.split(',')][-1]
        tests[test][size].append(data)


for test in tests:
    for size in tests[test]:
        tests[test][size] = np.mean(tests[test][size])
    tests[test] = np.array(list(tests[test].items()))

tests['ideal_estimated_cycles'] = np.array([tests[test][:,0], tests[test][:,0]/2]).T

for test in tests:
    if not 'cycle' in test:
        continue
    label = 'time (µs)' if test.split("_")[-1] in 'cycles' else 'instructions'
    if not 'ideal' in test:
        name = ' '.join(test.split('_')[:-1]) + ' test'
    else:
        name = ' '.join(test.split('_')[:-1])
    plt.plot(tests[test][:,0], tests[test][:,1]/10, label=name)
plt.title('Bandwidth benchmark, cycles')
plt.ylabel(label)
plt.xlabel("buffer size (bytes)")
plt.legend()
plt.savefig('bandwidth_benchmark_cycles.svg')
plt.close()


for test in tests:
    if 'cycle' in test:
        continue
    label = 'time (µs)' if test.split("_")[-1] in 'cycles' else 'instructions'
    name = ' '.join(test.split('_')[:-1]) + ' test'
    plt.plot(tests[test][:,0], tests[test][:,1]/10, label=name)
plt.ylabel(label)
plt.xlabel("buffer size (bytes)")
plt.title('Bandwidth benchmark, instructions')
plt.legend()
plt.savefig('bandwidth_benchmark_instructions.svg')
plt.close()


