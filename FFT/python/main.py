import numpy as np
from scipy.fft import fft, ifft
import matplotlib.pyplot as plt

def ImportArrayFromFile(file_path):
    fd = open(file_path, "r")
    file_content = fd.read()
    
    arr = []
    for item in file_content.split(" ")[:-1]:
        arr.append(float(item))

    return arr

def main():
    signal_file_path = "../data/custom_signal.dat"
    time_file_path = "../data/time.dat"

    data = ImportArrayFromFile(signal_file_path)
    time = ImportArrayFromFile(time_file_path)

    plt.plot(time, data)
    plt.savefig("../images/main.png")

if __name__ == '__main__':
    main()