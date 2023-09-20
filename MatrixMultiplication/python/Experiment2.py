import numpy as np
import matplotlib.pyplot as plt
import subprocess
from datetime import datetime
import pandas as pd

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
    # matrix_scales = list(range(1, 101))
    matrix_scales = list(range(1000, 1020))

    parallel_simple_matrix_multiplication_time_static = []
    parallel_simple_matrix_multiplication_time_dynamic = []
    parallel_simple_matrix_multiplication_time_guided = []

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(10):
            subprocess.run(["../parallel_simple_matrix_multiplication_static", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        parallel_simple_matrix_multiplication_time_static.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(10):
            subprocess.run(["../parallel_simple_matrix_multiplication_dynamic", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        parallel_simple_matrix_multiplication_time_dynamic.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(10):
            subprocess.run(["../parallel_simple_matrix_multiplication_guided", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.dat")
        parallel_simple_matrix_multiplication_time_guided.append(np.mean(tmp))
        CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name = r"../images/" + "ComparingTypesOfSchedele_" + date + r".jpg"

    fig = plt.figure()

    plt.title("Comparing different types of schedele")

    plt.scatter(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time_static), s=10)
    plt.plot(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time_static), label='stitic')

    plt.scatter(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time_dynamic), s=10)
    plt.plot(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time_dynamic), label='dynamic')

    plt.scatter(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time_guided), s=10)
    plt.plot(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time_guided), label='guided')
    
    plt.xlabel("matrix scale")
    plt.ylabel("time (ms)")
    plt.legend()
    plt.grid()
    fig.savefig(save_file_name)

if __name__ == '__main__':
    main()