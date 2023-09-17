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
    # matrix_scales = list(range(1, 101))
    matrix_scales = list(range(1000, 1020))

    simple_matrix_multiplication_time = []
    cache_friendly_matrix_multiplication_time = []
    parallel_simple_matrix_multiplication_time = []
    parallel_cache_friendly_matrix_multiplication_time = []

    for scale in matrix_scales:
        subprocess.run(["../simple_matrix_multiplication", f'{scale}'])
    simple_matrix_multiplication_time = ImportDataTimeFileContent("./time.dat")
    CleanFile("./time.dat")

    for scale in matrix_scales:
        subprocess.run(["../cache_friendly_matrix_multiplication", f'{scale}'])
    cache_friendly_matrix_multiplication_time = ImportDataTimeFileContent("./time.dat")
    CleanFile("./time.dat")

    # for scale in matrix_scales:
    #     subprocess.run(["../parallel_simple_matrix_multiplication", f'{scale}'])
    # parallel_simple_matrix_multiplication_time = ImportDataTimeFileContent("./time.dat")
    # CleanFile("./time.dat")

    # for scale in matrix_scales:
    #     subprocess.run(["../parallel_cache_friendly_matrix_multiplication", f'{scale}'])
    # parallel_cache_friendly_matrix_multiplication_time = ImportDataTimeFileContent("./time.dat")
    # CleanFile("./time.dat")

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name = r"../images/" + date + r".jpg"

    fig = plt.figure()

    plt.scatter(np.array(matrix_scales), np.array(simple_matrix_multiplication_time))
    plt.plot(np.array(matrix_scales), np.array(simple_matrix_multiplication_time), label='simple_matrix_multiplication')

    plt.scatter(np.array(matrix_scales), np.array(cache_friendly_matrix_multiplication_time))
    plt.plot(np.array(matrix_scales), np.array(cache_friendly_matrix_multiplication_time), label='cache_friendly_matrix_multiplication')

    # plt.scatter(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time))
    # plt.plot(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time), label='parallel_simple_matrix_multiplication')

    # plt.scatter(np.array(matrix_scales), np.array(parallel_cache_friendly_matrix_multiplication_time))
    # plt.plot(np.array(matrix_scales), np.array(parallel_cache_friendly_matrix_multiplication_time), label='parallel_cache_friendly_matrix_multiplication')

    plt.legend()
    plt.grid()
    fig.savefig(save_file_name)

if __name__ == '__main__':
    main()