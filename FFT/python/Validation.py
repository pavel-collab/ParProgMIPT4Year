import numpy as np
from scipy.fft import fft, ifft
import matplotlib.pyplot as plt

def ImportArrayFromFile(file_path):
    fd = open(file_path, "r")
    file_content = fd.read().split("\n")
    
    arr = []
    for item in file_content[1].split(" ")[:-1]:
        arr.append(float(item))

    return arr

def main():
    signal_file_path = "../data/custom_signal.dat"
    time_file_path = "../data/time.dat"
    fft_file_path = "../fft.dat"

    data = ImportArrayFromFile(signal_file_path)
    time = ImportArrayFromFile(time_file_path)
    fft_result = ImportArrayFromFile(fft_file_path)

    fft_data_real = fft(data).real


    # assert np.allclose(fft_data_real, fft_result)
    n = np.arange(len(time))
    # plt.stem(n, fft_data_real)
    # plt.stem(n, fft_result)
    plt.scatter(n, fft_data_real, s=3)
    plt.scatter(n, fft_result, s=3)
    plt.savefig("../images/validation.png")

if __name__ == '__main__':
    main()