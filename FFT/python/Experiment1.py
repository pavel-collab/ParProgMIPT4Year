import numpy as np
import matplotlib.pyplot as plt
import subprocess
from datetime import datetime

def ImportDataTimeFileContent(file_name: str) -> list:
    res_list = []
    fd = open(file_name, "r")
    content = fd.read()
    # the last symbol is \n
    # so, the last split str will be ""
    for s in content.split(" ")[:-1]:
        res_list.append(float(s))
    fd.close()  
    res = np.array(res_list)
    return res

def CleanFile(file_path):
    fd = open(file_path, "w")
    fd.write("")
    fd.close()

def main():
    cycles = 3

    proceses = range(1, 15+1)
    executable_naive_ft = "../naive_alg_time"
    executable_fft = "../fft_time"
    executable_parallel_fft = "../parallel_fft_time"

    signal_file_path = "../data/custom_signal.dat"

    naive_ft_time = []
    fft_time = []
    parallel_fft_time = []

    #------------------------------------------------------------------------------------
    # for proc in proceses:
    #     tmp = []
    #     for _ in range(cycles):
    #         subprocess.run([executable_naive_ft, f'{signal_file_path}', f'{proc}'])
    #     tmp = ImportDataTimeFileContent("./time.dat")
    #     naive_ft_time.append(np.mean(tmp))
    #     CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for proc in proceses:
        tmp = []
        for _ in range(cycles):
            subprocess.run([executable_fft, f'{signal_file_path}', f'{proc}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        fft_time.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for proc in proceses:
        tmp = []
        for _ in range(cycles):
            subprocess.run([executable_parallel_fft, f'{signal_file_path}', f'{proc}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        parallel_fft_time.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    
    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name = r"../images/" + "ParalleleAndConsistentAlgoritmsComparing_" + date + r".jpg"

    fig = plt.figure()

    plt.title("Comparing of all the algorithms")

    # plt.scatter(np.array(proceses), np.array(naive_ft_time), s=10)
    # plt.plot(np.array(proceses), np.array(naive_ft_time), label='naive ft')

    plt.scatter(np.array(proceses), np.array(fft_time), s=10)
    plt.plot(np.array(proceses), np.array(fft_time), label='fft')

    plt.scatter(np.array(proceses), np.array(parallel_fft_time), s=10)
    plt.plot(np.array(proceses), np.array(parallel_fft_time), label='parallel fft')

    plt.xlabel("proces number")
    plt.ylabel("time (ms)")

    plt.legend()
    plt.grid()
    fig.savefig(save_file_name)

if __name__ == '__main__':
    main()