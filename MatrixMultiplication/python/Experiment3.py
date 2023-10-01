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
    matrix_scales = [2**j for j in range(3, 14)]

    simple_matrix_multiplication_time = []
    cache_friendly_matrix_multiplication_time = []
    strassen_time = []

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(5):
            subprocess.run(["../simple_matrix_multiplication", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        simple_matrix_multiplication_time.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(5):
            subprocess.run(["../cache_friendly_matrix_multiplication", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        cache_friendly_matrix_multiplication_time.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(5):
            subprocess.run(["../strassen", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        strassen_time.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name = r"../images/" + "StrassenTest_" + date + r".jpg"

    fig = plt.figure()

    plt.title("Compare Strassen algorithm with others")

    plt.scatter(np.array(matrix_scales), np.array(simple_matrix_multiplication_time), s=10)
    plt.plot(np.array(matrix_scales), np.array(simple_matrix_multiplication_time), label='simple')

    plt.scatter(np.array(matrix_scales), np.array(cache_friendly_matrix_multiplication_time), s=10)
    plt.plot(np.array(matrix_scales), np.array(cache_friendly_matrix_multiplication_time), label='cache_friendly')

    plt.scatter(np.array(matrix_scales), np.array(strassen_time), s=10)
    plt.plot(np.array(matrix_scales), np.array(strassen_time), label='strassen')

    plt.xlabel("matrix scale")
    plt.ylabel("time (ms)")

    plt.legend()
    plt.grid()
    fig.savefig(save_file_name)

if __name__ == '__main__':
    main()