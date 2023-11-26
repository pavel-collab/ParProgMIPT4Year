import numpy as np
import argparse

def ImportDataFileContent(file_name: str):
    res = []
    with open(file_name) as file:
        lines = [line.rstrip() for line in file]
    
    for line in lines:
        if len(line) != 0: res.append(list(map(float, line.split(" "))))
    
    return np.array(res)

def CleanFile(file_path):
    fd = open(file_path, "w")
    fd.write("")
    fd.close()

def main():
    # parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", type=str, help="Path to the file with data to check out")
    
    args = parser.parse_args()

    if args.file == None:
        print("[-] Error, expected path to file with data to check out.")
        return
    
    path_to_data_file = args.file
    valid_data_file_path = "../valid_data.txt"

    valid_data = ImportDataFileContent(valid_data_file_path)
    data = ImportDataFileContent(path_to_data_file)

    if not np.isclose(data.all(), valid_data.all()):
        print("[-] Error. Data file is not correct")
    else:
        print("[+] Success")

if __name__ == '__main__':
    main()