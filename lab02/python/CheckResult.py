import numpy as np

def ImportDataFileContent(file_name: str):
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
    check_result_files = [
        "../check_result/check_result_task1.txt",
        "../check_result/check_result_task2.txt",
        "../check_result/check_result_task3.txt"
    ]

    result_files = [
        "../result_task1.txt",
        "../result_task2.txt",
        "../result_task3.txt"
    ]

    for i in range(3):
        check_result_data = ImportDataFileContent(check_result_files[i])
        result_data = ImportDataFileContent(result_files[i])

        if result_data == check_result_data:
            print(f"Task {i+1} is ok")
        else:
            print(f"Task {i+1} is wrong")
            print(f"The result is {result_data}")
            print(f"But correct result is {check_result_data}")

if __name__ == '__main__':
    main()