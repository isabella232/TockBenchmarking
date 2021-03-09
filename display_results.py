import sys
import numpy as np
import matplotlib.pyplot as plt

class Experiment:
    def __init__(self):
        self.name = ""
        self.description = ""
        self.data = []
        self.labels = []

    def extratMetadata(self, row):
        
        if 'benchmark' in row[0]:
            self.name = row[0].replace(":", "").replace("benchmark", "").strip()

    def addRow(self, row):
        if len(row) < 1:
            return

        if len(row) == 1:
            self.extratMetadata(row)
            return

        try:
            row_data = []
            for element in row:
                int_val = int(element)
                row_data.append(int_val)
            self.data.append(row_data)
        except Exception:
            self.labels = row

    def getAverages(self):
        if not self.isValid():
            return
        data_array = np.asarray(self.data)
        averages = np.average(data_array, axis=0)
        return averages[1:]

    def getErrorRange(self):
        if not self.isValid():
            return
        data_array = np.asarray(self.data)
        averages = np.average(data_array, axis=0)
        deltas = data_array - averages
        abs_deltas = np.abs(deltas)
        max_deltas = np.max(abs_deltas, axis=0)
        return max_deltas[1:]

    def isValid(self):
        return len(self.data) > 0

    def getCloned(self):
        new_experiment = Experiment()
        new_experiment.labels = self.labels
        new_experiment.name = self.name
        new_experiment.description = self.description
        return new_experiment

    def getAveragedData(self):
        block = self.data[0][0]
        sum = 0
        averages = []
        blocks = []
        count = 0
        for datum in self.data:
            if block != datum[0]:
                averages.append(sum / count)
                blocks.append(block)
                sum = 0
                count = 0
                block = datum[0]
            sum += datum[2]
            count += 1
        averages.append(sum / count)
        blocks.append(block)
        return averages, blocks


def isNewExperiment(row):
    if len(row) != 1:
        return False
    if "benchmark" in row[0]:
        return True
    return False

def plotExperiments(experiments, labels=None, title="", ylabel=""):
    values_count = len(experiments[0].labels) - 1
    values = []
    errors = []
    build_labels = not labels
    if not labels:
        labels = []
    for experiment in experiments:
        values.append(experiment.getAverages())
        errors.append(experiment.getErrorRange())
        if build_labels:
            labels.append(experiment.name)
    
    numpy_values = np.asarray(values)
    numpy_errors = np.asarray(errors)
    width = 1 / (values_count + 1)
    numpy_values[0,:] *= 0.1
    print(numpy_values)
    print(numpy_values[:,0])
    for i in range(len(numpy_values[0,:])):
        print(i, numpy_values[:][i])
        plt.bar(np.arange(len(numpy_values[:,i])) + i * width, numpy_values[:,i], width=width, yerr=numpy_errors[:,i])

    plt.title("Time measurement overhead")
    plt.ylabel("microseconds / instructions")
    if labels:
        plt.xticks(range(len(values)), labels)
    plt.show()

"""
\begin{table}[h!]
\centering
\begin{tabular}{||c c c c||} 
 \hline
 Col1 & Col2 & Col2 & Col3 \\ [0.5ex] 
 \hline\hline
 1 & 6 & 87837 & 787 \\ 
 2 & 7 & 78 & 5415 \\
 3 & 545 & 778 & 7507 \\
 4 & 545 & 18744 & 7560 \\
 5 & 88 & 788 & 6344 \\ [1ex] 
 \hline
\end{tabular}
\caption{Table to test captions and labels}
\label{table:1}
\end{table}
"""
def convertToTex(experiments):
    width = len(experiments)
    height = len(experiments[0].labels)
    header = " &"
    # for experiment in experiments:

def graphExperiment(experiment):
    y, x = experiment.getAveragedData()
    x = np.asarray(x)
    y = np.asarray(y)
    r = np.corrcoef(x, y)
    print(r)
    plt.title("Time to read flash")
    plt.ylabel("milliseconds")
    plt.xlabel("kb in icache")
    plt.plot(x/1024, y/10000)
    plt.show()


def processFile(filename, labels=None, special_handler=None, title="", ylabel=""):
    path_root = sys.argv[0].replace("display_results.py", "")
    experiment = Experiment()
    experiments = []
    with open(path_root + "c/data/{}".format(filename), newline="") as csvfile:
        for line in csvfile:

            row = line.split(",")
            if not isNewExperiment(row):
                experiment.addRow(row)
            else:
                if experiment.isValid():
                    experiments.append(experiment)
                experiment = Experiment()
                experiment.addRow(row)

    if experiment.isValid():
        experiments.append(experiment)

    # plotExperiments(experiments, labels=labels, title=title, ylabel=ylabel)
    graphExperiment(experiments[0])

processFile("icache.log", labels=["microseconds", "instructions"], title="Time measurement overhead", ylabel="microseconds / instructions")
