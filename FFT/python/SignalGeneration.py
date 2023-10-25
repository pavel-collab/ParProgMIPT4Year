import numpy as np
from scipy import signal as sg
import matplotlib.pyplot as plt

def GenMeandr2File(file_path: str, ampl: float, freq: int, time, offload=0, duty=0.5):
    with open(file_path, "a") as fd:
        omega = 2*np.pi*freq
        meandr = ampl * sg.square(omega*time, duty) 
        if offload != 0:
            meandr += np.ones(len(time))*offload

        fd.write(str(len(meandr)) + "\n")
        for data in meandr:
            fd.write(str(data) + " ")

def GenSin2File(file_path: str, ampl: float, freq: int, time, offload=0):
    with open(file_path, "a") as fd:
        omega = 2*np.pi*freq
        signal = ampl * np.sin(omega*time) 
        if offload != 0:
            signal += np.ones(len(time))*offload

        fd.write(str(len(signal)) + "\n")
        for data in signal:
            fd.write(str(round(data, 4)) + " ")

def GenCustomSignal2File(file_path: str, ampl: float, freq: int, time, offload=0, duty=0.5):
    with open(file_path, "a") as fd:
        omega = 2*np.pi*freq

        meandr = ampl * sg.square(omega*time, duty) 
        if offload != 0:
            meandr += np.ones(len(time))*offload

        sin_signal = (ampl*2) * np.sin((omega*10) * time)

        signal = meandr*sin_signal
        fd.write(str(len(meandr)) + "\n")
        for data in signal:
            fd.write(str(round(data, 4)) + " ")

def PutArray2File(file_path, arr):
    with open(file_path, "a") as fd:
        fd.write(str(len(arr)) + "\n")
        for data in arr:
            fd.write(str(round(data, 4)) + " ")

def main():
    T_from = -10 # sec
    T_to = 10 # sec
    freq = 1 # Hz

    samples_list = [2**x for x in range(5, 20+1)]

    for samples_amount in samples_list:
        time = np.linspace(T_from, T_to, samples_amount)
        time_file_path = "../data/time" + f"_{samples_amount}_" + ".dat"
        PutArray2File(time_file_path, time)

        custom_signal_file_path = "../data/custom_signal" + f"_{samples_amount}_" + ".dat"
        GenCustomSignal2File(custom_signal_file_path, 0.5, freq, time, 0.5)

if __name__ == '__main__':
    main()