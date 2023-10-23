import numpy as np
from scipy import signal as sg

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

        fd.write(str(len(meandr)) + "\n")
        for data in meandr*sin_signal:
            fd.write(str(round(data, 4)) + " ")

def PutArray2File(file_path, arr):
    with open(file_path, "a") as fd:
        fd.write(str(len(arr)) + "\n")
        for data in arr:
            fd.write(str(round(data, 4)) + " ")

def main():
    samples_amount = 2**20

    time = np.linspace(-10, 10, samples_amount)
    time_file_path = "../data/time.dat"
    PutArray2File(time_file_path, time)

    meandr_file_path = "../data/meandr.dat"
    GenMeandr2File(meandr_file_path, 0.5, 1, time, 0.5)

    sin_file_path = "../data/sin.dat"
    GenSin2File(sin_file_path, 1, 1, time)

    custom_signal_file_path = "../data/custom_signal.dat"
    GenCustomSignal2File(custom_signal_file_path, 0.5, 1, time, 0.5)

if __name__ == '__main__':
    main()