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
    proceses = [1, 2, 3, 4, 5]
    executable_file_bubble = "../bubble"
    executable_file_quick = "../quick"
    test_file_path = "../tests/test1048576.dat"

    depth = 268

    #------------------------------------------------------------------------------------
    for proc in proceses:
        subprocess.run([f'{executable_file_bubble}', f'{test_file_path}', f'{proc}', f'{depth}'])            
    bubble_time = ImportDataTimeFileContent("./time.dat")
    CleanFile("./time.dat")

    for proc in proceses:
        subprocess.run([f'{executable_file_quick}', f'{test_file_path}', f'{proc}', f'{depth}'])            
    quick_time = ImportDataTimeFileContent("./time.dat")
    CleanFile("./time.dat")
    #------------------------------------------------------------------------------------

    acceleration_bubbe = [bubble_time[0] for _ in range(len(bubble_time))] / bubble_time
    acceleration_quick = [quick_time[0] for _ in range(len(quick_time))] / quick_time

    efficiency_bubble = acceleration_bubbe / proceses
    efficiency_quick = acceleration_quick / proceses

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_name_acceleration = r"../images/" + "acceleration_" + date + r".jpg"
    save_file_name_efficiency = r"../images/" + "efficiency_" + date + r".jpg"

    fig1 = plt.figure()

    plt.title(f"Acceleration (depth = {depth})")

    plt.scatter(np.array(proceses), np.array(acceleration_bubbe), s=10)
    plt.plot(np.array(proceses), np.array(acceleration_bubbe), label='bubble')

    plt.scatter(np.array(proceses), np.array(acceleration_quick), s=10)
    plt.plot(np.array(proceses), np.array(acceleration_quick), label='quick')

    plt.xlabel("proc num")
    plt.ylabel("acceleration")

    plt.legend()
    plt.grid()
    fig1.savefig(save_file_name_acceleration)

    #========================================================================================

    fig1 = plt.figure()

    plt.title(f"Efficiency (depth = {depth})")

    plt.scatter(np.array(proceses), np.array(efficiency_bubble), s=10)
    plt.plot(np.array(proceses), np.array(efficiency_bubble), label='bubble')

    plt.scatter(np.array(proceses), np.array(efficiency_quick), s=10)
    plt.plot(np.array(proceses), np.array(efficiency_quick), label='quick')

    plt.xlabel("proc num")
    plt.ylabel("efficiency")

    plt.legend()
    plt.grid()
    fig1.savefig(save_file_name_efficiency)

if __name__ == '__main__':
    main()