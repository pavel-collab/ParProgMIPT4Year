import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import argparse
import subprocess
from datetime import datetime

def ImportDataFileContent(file_name: str) -> list:
    res_list = []
    fd = open(file_name, "r")
    content = fd.read()
    # the last symbol is \n
    # so, the last split str will be ""
    for s1 in content.split("\n")[:-1]:
        sub_res = []
        for s2 in s1.split(" ")[:-1]:
            sub_res.append(float(s2))
        res_list.append(sub_res)
    fd.close()  
    res = np.array(res_list)
    return res.T

def CleanFile(file_path):
    fd = open(file_path, "w")
    fd.write("")
    fd.close()

def main():
    # executable_file = "../bubble"
    executable_file = "../quick"
    test_file_path = "../tests/bad_case100000.dat"

    time_file_path = "./time.dat"

    proces_amnt_list = list(range(1, 8+1))
    recursion_depth_list = list(range(50, 500+1))

    for proc_n in proces_amnt_list:
        for depth in recursion_depth_list:
            subprocess.run([f'{executable_file}', f'{test_file_path}', f'{proc_n}', f'{depth}'])
        fd = open(time_file_path, "a")
        fd.write("\n")
        fd.close()

    data = ImportDataFileContent(time_file_path)

    proc_grid, depth_grid = np.meshgrid(proces_amnt_list, recursion_depth_list)

    date = datetime.strftime(datetime.now(), "%d.%m.%Y-%H.%M.%S")
    save_file_path = r"../images/" + date + r".jpg"

    proc_point_amnt = np.shape(data)[1]
    depth_point_amnt = np.shape(data)[1]

    min_time = data.min()
    min_time_idx = data.argmin()
    critical_proc_amnt_idx = min_time_idx % depth_point_amnt
    critical_depth_idx = int(min_time_idx / depth_point_amnt)

    fig = plt.figure()
    plt.title(f'Min is {min_time} with proc {proces_amnt_list[critical_proc_amnt_idx]} and depth {recursion_depth_list[critical_depth_idx]}')
    ax_3d = fig.add_subplot(projection="3d")
    ax_3d.plot_surface(proc_grid, depth_grid, data)
    ax_3d.plot_wireframe(proc_grid, depth_grid, data)
    ax_3d.set_xlabel("proc")
    ax_3d.set_ylabel("depth")
    ax_3d.set_zlabel("t, ms")
    fig.savefig(f'{save_file_path}')
    plt.show()

if __name__ == '__main__':
    main()