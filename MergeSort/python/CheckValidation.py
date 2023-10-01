import argparse
import subprocess

def ImportArrayFromFile(file_path):
    fd = open(file_path, "r")
    file_content = fd.read().split("\n")
    
    arr = []
    for item in file_content[1].split(" ")[:-1]:
        arr.append(int(item))

    return arr

def ArrCmp(arr1, arr2):
    if (len(arr1) != len(arr2)):
        print("[-] Arrays have not equal length")
        print(f'array arr1 has length {len(arr1)} and array arr2 has length {len(arr2)}')
        return False
    N = len(arr1)
    for i in range(N):
        if (arr1[i] != arr2[i]):
            print("[-] Arrays have not equal elements on one index")
            print(f'arr1[{i}] = {arr1[i]} and arr2[{i}] = {arr2[i]}')
            return False
    return True

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file_path", help="*description*")
    args = parser.parse_args()

    N_proc = 3

    file_path = ""
    if args.file_path != None:
        file_path = args.file_path
    else:
        print("[-] Error, no file with data")
        raise RuntimeError
    
    subprocess.run(["../a.out", f'{file_path}', f'{N_proc}'])

    result = ImportArrayFromFile("./result.txt")

    valid_array = ImportArrayFromFile(file_path)
    valid_array.sort()
    
    if (ArrCmp(result, valid_array) == True):
        print("Result is valid")


if __name__ == '__main__':
    main()
