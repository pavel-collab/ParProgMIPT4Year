import numpy as np
import matplotlib.pyplot as plt
import subprocess
from datetime import datetime
import argparse

from pathlib import Path
import os

#TODO: using argparse make the implementation, that get binaries from the command line (but in this case we will not set a plot tile)

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

# def CheckPaths():
#     current_dir = os.getcwd()
    
#     bin1 = (Path(current_dir) / ".." / "simple_matrix_multiplication_static").absolute()
#     bin2 = (Path(current_dir) / ".." / "cache_friendly_matrix_multiplication").absolute()
#     bin3 = (Path(current_dir) / ".." / "parallel_simple_matrix_multiplication").absolute()
#     bin4 = (Path(current_dir) / ".." / "parallel_cache_friendly_matrix_multiplication").absolute()

#     if (
#         bin1.exists() == False or
#         bin2.exists() == False or
#         bin3.exists() == False or
#         bin4.exists() == False 
#         ):
#         print("[-] Error. bin is not exists")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--vectorization", help="*description*", action="store_true")
    args = parser.parse_args()
    
    if args.vectorization != False:
        matrix_scales = [2**j for j in range(3, 13)]
    else:
        # matrix_scales = list(range(4, 101))
        matrix_scales = list(range(1000, 2000+1, 50))

    simple_matrix_multiplication_time = []
    cache_friendly_matrix_multiplication_time = []
    parallel_simple_matrix_multiplication_time = []
    parallel_cache_friendly_matrix_multiplication_time = []

    vectorization_matrix_multiplication_time = []
    parallel_vectorization_matrix_multiplication_time = []

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(5):
            subprocess.run(["../simple_matrix_multiplication", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.txt")
        simple_matrix_multiplication_time.append(np.mean(tmp))
        CleanFile("./time.txt")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(5):
            subprocess.run(["../cache_friendly_matrix_multiplication", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.txt")
        cache_friendly_matrix_multiplication_time.append(np.mean(tmp))
        CleanFile("./time.txt")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(5):
            subprocess.run(["../parallel_simple_matrix_multiplication_static", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.txt")
        parallel_simple_matrix_multiplication_time.append(np.mean(tmp))
        CleanFile("./time.txt")
    #------------------------------------------------------------------------------------

    #------------------------------------------------------------------------------------
    for scale in matrix_scales:
        tmp = []
        for _ in range(5):
            subprocess.run(["../parallel_cache_friendly_matrix_multiplication", f'{scale}'])
        tmp = ImportDataTimeFileContent("./time.txt")
        parallel_cache_friendly_matrix_multiplication_time.append(np.mean(tmp))
        CleanFile("./time.txt")
    #------------------------------------------------------------------------------------

    if args.vectorization != False:
        #------------------------------------------------------------------------------------
        for scale in matrix_scales:
            tmp = []
            for _ in range(5):
                subprocess.run(["../vectorized_matrix_multiplication", f'{scale}'])
            tmp = ImportDataTimeFileContent("./time.txt")
            vectorization_matrix_multiplication_time.append(np.mean(tmp))
            CleanFile("./time.txt")
        #------------------------------------------------------------------------------------

        #------------------------------------------------------------------------------------
        for scale in matrix_scales:
            tmp = []
            for _ in range(5):
                subprocess.run(["../parallel_vectorized_matrix_multiplication", f'{scale}'])
            tmp = ImportDataTimeFileContent("./time.txt")
            parallel_vectorization_matrix_multiplication_time.append(np.mean(tmp))
            CleanFile("./time.txt")
        #------------------------------------------------------------------------------------

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name = r"../images/" + "ParalleleAndConsistentAlgoritmsComparing_" + date + r".jpg"

    fig = plt.figure()

    plt.title("Comparing of all the algorithms")

    plt.scatter(np.array(matrix_scales), np.array(simple_matrix_multiplication_time), s=10)
    plt.plot(np.array(matrix_scales), np.array(simple_matrix_multiplication_time), label='simple')

    plt.scatter(np.array(matrix_scales), np.array(cache_friendly_matrix_multiplication_time), s=10)
    plt.plot(np.array(matrix_scales), np.array(cache_friendly_matrix_multiplication_time), label='cache_friendly')

    plt.scatter(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time), s=10)
    plt.plot(np.array(matrix_scales), np.array(parallel_simple_matrix_multiplication_time), label='parallel_simple_matrix_multiplication')

    plt.scatter(np.array(matrix_scales), np.array(parallel_cache_friendly_matrix_multiplication_time), s=10)
    plt.plot(np.array(matrix_scales), np.array(parallel_cache_friendly_matrix_multiplication_time), label='parallel_cache_friendly')

    if args.vectorization != False:
        plt.scatter(np.array(matrix_scales), np.array(vectorization_matrix_multiplication_time), s=10)
        plt.plot(np.array(matrix_scales), np.array(vectorization_matrix_multiplication_time), label='vectorization')

        plt.scatter(np.array(matrix_scales), np.array(parallel_vectorization_matrix_multiplication_time), s=10)
        plt.plot(np.array(matrix_scales), np.array(parallel_vectorization_matrix_multiplication_time), label='parallel_vectorization')

    plt.xlabel("matrix scale")
    plt.ylabel("time (ms)")

    plt.legend()
    plt.grid()
    fig.savefig(save_file_name)

if __name__ == '__main__':
    main()