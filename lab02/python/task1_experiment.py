import numpy as np
import matplotlib.pyplot as plt
import subprocess
from datetime import datetime
import argparse

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
    proces_number = 2
    # scales = [_ for _ in range(5000, 40000, 5000)]
    scales = [_ for _ in range(1000, 10000, 1000)]

    for scale in scales:
        subprocess.run(["../task1_consistent", f'{scale}', f'{scale}'])
    seq_time = ImportDataTimeFileContent("./time.txt")
    CleanFile("./time.txt")

    for scale in scales:
        subprocess.run([f"mpiexec -n {proces_number} ../task1_parallel", f'{scale}', f'{scale}'])
    inner_time = ImportDataTimeFileContent("./time.txt")
    CleanFile("./time.txt")

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name = r"../images/" + "task1_" + date + r".jpg"

    fig = plt.figure()

    plt.title("Comparing of all the algorithms task1")

    plt.scatter(np.array(scales), np.array(seq_time), s=10)
    plt.plot(np.array(scales), np.array(seq_time), label='seq')

    plt.scatter(np.array(scales), np.array(inner_time), s=10)
    plt.plot(np.array(scales), np.array(inner_time), label='inner')

    plt.xlabel("ISIZE (ISIZE = JSIZE)")
    plt.ylabel("time (ms)")

    plt.legend()
    plt.grid()
    fig.savefig(save_file_name)

if __name__ == '__main__':
    main()