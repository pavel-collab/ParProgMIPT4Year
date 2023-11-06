// it need to have cl headers for this work
// if you have not any use: sudo apt-get install opencl-headers
// and visible dynamic opencl libraries sudo apt install ocl-icd-opencl-dev

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

void checkErr(cl_int err, int num) {
	if (CL_SUCCESS != err) {
		printf("OpenCL error(%d) at %d\n", err, num - 1);
	}
}

int main(int argc,char ** argv) {
	cl_device_id* device;
	cl_platform_id platform;

    cl_uint ret_num_platforms;

	cl_int err;
	cl_uint NumDevice;

	// Выберите первую платформу
	err = clGetPlatformIDs(0, &platform, &ret_num_platforms);
	checkErr(err, __LINE__);
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &NumDevice);
	checkErr(err, __LINE__);
	device = (cl_device_id*)malloc(sizeof(cl_device_id) * NumDevice);
	// Выбор устройства с графическим процессором
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, NumDevice, device, NULL);

	checkErr(err, __LINE__);
	for (int i = 0; i < NumDevice; i++) {
		// Запросим имя устройства
		char buffer[110];
		err = clGetDeviceInfo(device[i], CL_DEVICE_NAME, 100, buffer, NULL);
		checkErr(err, __LINE__);
		printf("Device Name:%s\n", buffer);
		// Запрос максимального количества вычислительных единиц устройства
		cl_uint UnitNum;
		err = clGetDeviceInfo(device[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &UnitNum, NULL);
		checkErr(err, __LINE__);
		printf("Compute Units Number: %d\n", UnitNum);
		// Запрос частоты ядра устройства
		cl_uint frequency;
		err = clGetDeviceInfo(device[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &frequency, NULL);
		checkErr(err, __LINE__);
		printf("Device Frequency: %d(MHz)\n", frequency);
		// Запрос общего объема памяти устройства
		cl_ulong GlobalSize;
		err = clGetDeviceInfo(device[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &GlobalSize, NULL);
		checkErr(err, __LINE__);
		printf("Device Global Size: %0.0f(MB)\n", (float)GlobalSize/1024/1024);
		// Запрос строки глобального кэша памяти устройства
		cl_uint GlobalCacheLine;
		err = clGetDeviceInfo(device[i], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint), &GlobalCacheLine, NULL);
		checkErr(err, __LINE__);
		printf("Device Global CacheLine: %d(Byte)\n", GlobalCacheLine);
		// Запрос версии OpenCL, поддерживаемой устройством
		char DeviceVersion[110];
		err = clGetDeviceInfo(device[i], CL_DEVICE_VERSION, 100, DeviceVersion, NULL);
		checkErr(err, __LINE__);
		printf("Device Version:%s\n", DeviceVersion);
		// Запрос расширения устройства
		char* DeviceExtensions;
		size_t ExtenNum;
		err = clGetDeviceInfo(device[i], CL_DEVICE_EXTENSIONS, 0, NULL, &ExtenNum);
		checkErr(err, __LINE__);
		DeviceExtensions = (char*)malloc(ExtenNum);
		err = clGetDeviceInfo(device[i], CL_DEVICE_EXTENSIONS, ExtenNum, DeviceExtensions, NULL);
		checkErr(err, __LINE__);
		printf("Device Extensions: %s\n", DeviceExtensions);
		free(DeviceExtensions);
	}
	free(device);
	return 0;
	
}
