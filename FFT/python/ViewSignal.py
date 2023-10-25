import matplotlib.pyplot as plt
import argparse
from sys import exit

def ImportArrayFromFile(file_path):
    fd = open(file_path, "r")
    file_content = fd.read().split("\n")
    
    arr = []
    for item in file_content[1].split(" ")[:-1]:
        arr.append(float(item))

    n_samples = int(file_content[0])
    return (n_samples, arr)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-tf", "--time_file", type=str, help="The name of file with time")
    parser.add_argument("-df", "--data_file", type=str, help="The name of file with data")
    args = parser.parse_args()

    if args.time_file != None and  args.data_file != None:
        signal_file_path = args.data_file
        time_file_path = args.time_file
    else:
        print("[-] Error, please set the path to the data file and time file. Usage python3 Validation.py -df data_file_path -tf time_file_path")
        exit(1)

    n_data_samples, data = ImportArrayFromFile(signal_file_path)
    n_time_samples, time = ImportArrayFromFile(time_file_path)
    assert n_data_samples == n_time_samples

    #! Be careful. This periode of time must be the same in SignalGeneration.py
    T_from = -10 # sec
    T_to = 10 # sec

    # Plot result signal
    samples_for_one_periode = int(n_data_samples / T_to - T_from)

    #--------------------------------------------------------------------------------

    plt.figure(figsize=(27, 7))

    plt.subplot(1, 2, 1)
    
    plt.plot(time, data)

    plt.title('Signal in time')
    plt.xlabel('time in seconds') 
    plt.ylabel('Signal value') 
    plt.grid()

    #--------------------------------------------------------------------------------

    plt.subplot(1, 2, 2)

    plt.plot(time[:samples_for_one_periode], data[:samples_for_one_periode])

    plt.title('One signal periode meandr')
    plt.xlabel('time in seconds') 
    plt.ylabel('Signal value') 
    plt.grid()

    #--------------------------------------------------------------------------------
    plt.savefig("../images/signal.png")

if __name__ == '__main__':
    main()