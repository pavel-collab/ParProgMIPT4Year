import numpy as np
from scipy.fft import fft, ifft
import matplotlib.pyplot as plt
import argparse
from sys import exit

def ImportArrayFromFile(file_path):
    fd = open(file_path, "r")
    file_content = fd.read().split("\n")
    
    arr = []
    for item in file_content[1].split(" ")[:-1]:
        arr.append(float(item))

    return arr

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", type=str, help="The name of file with data")
    parser.add_argument("-p", "--plot", help="*description*", action="store_true")
    args = parser.parse_args()

    if args.file != None:
        signal_file_path = args.file
    else:
        print("[-] Error, please set the path to the data file. Usage python3 Validation.py -f data_file_path")
        exit(1)

    
    real_fft_file_path = "../fft_real.dat"
    imag_fft_file_path = "../fft_imag.dat"

    data = ImportArrayFromFile(signal_file_path)
    fft_result_real = ImportArrayFromFile(real_fft_file_path)
    fft_result_imag = ImportArrayFromFile(imag_fft_file_path)

    fft_data_real = fft(data).real
    fft_data_imag = fft(data).imag

    n = np.arange(len(fft_result_real))

    assert len(fft_result_real) == len(fft_result_imag)
    assert len(fft_result_real) == len(fft_data_real)

    #! all close is not valid. May be we use low accuracy (4 decimal symbols)
    #TODO: rlot -- max tolerance difference between values, that can be. Check it works
    if not np.allclose(fft_data_real, fft_result_real, rtol=1e-3) or not np.allclose(fft_data_imag, fft_result_imag, rtol=1e-3):
        print("[-] Error. FFT result is invalid")
    else:
        print("[+] Success")

    if args.plot != False:
        #--------------------------------------------------------------------------------

        plt.figure(figsize=(27, 7))

        plt.subplot(1, 2, 1)
        
        plt.scatter(n, fft_data_real, s=3, label='correct fft')
        plt.scatter(n, fft_result_real, s=3, label='c++ alg')
        
        plt.title('Validation of the fft real part')
        # plt.xlabel() 
        # plt.ylabel() 
        plt.grid()
        plt.legend()

        #--------------------------------------------------------------------------------

        plt.subplot(1, 2, 2)

        plt.scatter(n, fft_data_imag, s=3, label='correct fft')
        plt.scatter(n, fft_result_imag, s=3, label='c++ alg')

        plt.title('Validation of the fft imag part')
        # plt.ylabel()
        # plt.xlabel()
        plt.grid()
        plt.legend()

        #--------------------------------------------------------------------------------
        plt.savefig("../images/validation.png")

if __name__ == '__main__':
    main()