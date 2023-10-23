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
    proceses = range(1, 15+1)
    executable_parallel_fft = "../parallel_fft_time"
    signal_file_path = "../data/custom_signal.dat"

    #------------------------------------------------------------------------------------
    for proc in proceses:
        subprocess.run([f'{executable_parallel_fft}', f'{signal_file_path}', f'{proc}'])            
    parallel_fft_time = ImportDataTimeFileContent("./time.dat")
    CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    acceleration_parallel = [parallel_fft_time[0] for _ in range(len(parallel_fft_time))] / parallel_fft_time
    efficiency_parallel = acceleration_parallel / proceses

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name_acceleration = r"../images/" + "acceleration_" + date + r".jpg"
    save_file_name_efficiency = r"../images/" + "efficiency_" + date + r".jpg"

    fig1 = plt.figure()

    plt.title(f"Acceleration of parallel implementation")

    plt.scatter(np.array(proceses), np.array(acceleration_parallel), s=10)
    plt.plot(np.array(proceses), np.array(acceleration_parallel))

    plt.xlabel("proc num")
    plt.ylabel("acceleration")

    plt.grid()
    fig1.savefig(save_file_name_acceleration)

    #========================================================================================

    fig1 = plt.figure()

    plt.title(f"Efficiency of parallel")

    plt.scatter(np.array(proceses), np.array(efficiency_parallel), s=10)
    plt.plot(np.array(proceses), np.array(efficiency_parallel))

    plt.xlabel("proc num")
    plt.ylabel("efficiency")

    plt.grid()
    fig1.savefig(save_file_name_efficiency)

if __name__ == '__main__':
    main()