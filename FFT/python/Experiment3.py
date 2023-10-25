import numpy as np
import matplotlib.pyplot as plt
import subprocess
from datetime import datetime
import os.path

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

    #! Fix prosesses amount
    proceses = 6
    executable_naive_ft = "../naive_alg_time"
    executable_fft = "../fft_time"
    executable_parallel_fft = "../parallel_fft_time"

    naive_ft_time = []
    fft_time = []
    parallel_fft_time = []

    samples_list = [2**x for x in range(5, 20+1)]
    data_file_list = []
    for samples in samples_list:
        custom_signal_file_path = "../data/custom_signal" + f"_{samples}_" + ".dat"
        assert os.path.exists(custom_signal_file_path)
        data_file_list.append(custom_signal_file_path)

    #------------------------------------------------------------------------------------
    # for file in data_file_list:
    #     tmp = []
    #     for _ in range(cycles):
    #         subprocess.run([executable_naive_ft, f'{file}', f'{proceses}'])
    #     tmp = ImportDataTimeFileContent("./time.dat")
    #     naive_ft_time.append(np.mean(tmp))
    #     CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for file in data_file_list:
        tmp = []
        for _ in range(cycles):
            subprocess.run([executable_fft, f'{file}', f'{proceses}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        fft_time.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for file in data_file_list:
        tmp = []
        for _ in range(cycles):
            subprocess.run([executable_parallel_fft, f'{file}', f'{proceses}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        parallel_fft_time.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name = r"../images/" + "TimeComparing_" + date + r".jpg"

    fig1 = plt.figure()

    plt.title(f"Acceleration of parallel implementation")

    # plt.scatter(samples_list, np.array(naive_ft_time), s=10)
    # plt.plot(samples_list, np.array(naive_ft_time), label='naive alg')

    plt.scatter(samples_list, np.array(fft_time), s=10)
    plt.plot(samples_list, np.array(fft_time), label='fft')

    plt.scatter(samples_list, np.array(parallel_fft_time), s=10)
    plt.plot(samples_list, np.array(parallel_fft_time), label='parallel fft')

    plt.xlabel("fft base (samples amount)")
    plt.ylabel("t (ms)")

    plt.grid()
    plt.legend()
    fig1.savefig(save_file_name)

if __name__ == '__main__':
    main()