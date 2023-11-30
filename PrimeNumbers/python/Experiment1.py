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
    scales = range(100000, 5000000, 100000)
    executable_file_path = "../time"
    time_file_name = "time.txt"
    proc_num = 4

    for scale in scales:
        subprocess.run([f'{executable_file_path}', f'{scale}', f'{1}'])
    seq_time = ImportDataTimeFileContent(time_file_name)
    CleanFile(time_file_name)

    for scale in scales:
        subprocess.run([f'{executable_file_path}', f'{scale}', f'{proc_num}'])
    parallel_time = ImportDataTimeFileContent(time_file_name)
    CleanFile(time_file_name)

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name_time = r"../images/" + "time_" + date + r".jpg"

    fig0 = plt.figure()

    plt.scatter(np.array(scales), np.array(seq_time), s=10)
    plt.plot(np.array(scales), np.array(seq_time), label=f'{1} thread')

    plt.scatter(np.array(scales), np.array(parallel_time), s=10)
    plt.plot(np.array(scales), np.array(parallel_time), label=f'{proc_num} threads')

    plt.xlabel("scales")
    plt.ylabel("time, ms")

    plt.legend()
    plt.grid()
    fig0.savefig(save_file_name_time)

if __name__ == '__main__':
    main()