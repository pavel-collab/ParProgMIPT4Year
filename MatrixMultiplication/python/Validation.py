import numpy as np
import matplotlib.pyplot as plt
import subprocess
from datetime import datetime
import argparse

# def ImportDataTimeFileContent(file_name: str):
#     res = []
#     with open(file_name) as file:
#         lines = [line.rstrip() for line in file]
    
#     for line in lines:
#         res.append(list(map(int, line.split(" "))))
    
#     return np.array(res)

def ImportDataTimeFileContent(file_name: str):
    res = []
    with open(file_name) as file:
        lines = [line.rstrip() for line in file]
    
    for line in lines:
        res.append(list(map(float, line.split(" "))))
    
    return np.array(res)

def CleanFile(file_path):
    fd = open(file_path, "w")
    fd.write("")
    fd.close()

def main():
    matrix1_file_name = "../matrix1.dat"
    matrix2_file_name = "../matrix2.dat"
    multiplication_result_file_name = "../multiplication_result.dat"

    matrix1 = ImportDataTimeFileContent(matrix1_file_name)
    matrix2 = ImportDataTimeFileContent(matrix2_file_name)
    result = ImportDataTimeFileContent(multiplication_result_file_name)

    validation = np.dot(matrix1, matrix2)
    check_result = (validation == result)
    if False in check_result:
        print("[-] Error")
        print("The right answer is")
        print(validation)
        print("But the real answer is")
        print(result)
    else:
        print("[+] Success")

if __name__ == '__main__':
    main()