#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    unsigned int i, j;                              

    cl_platform_id platforms[32];//An array to hold the IDs of all the platforms, hopefuly there won't be more than 32
    cl_uint num_platforms;//This number will hold the number of platforms on this machine
    char vendor[1024];//This strirng will hold a platforms vendor

    cl_device_id devices[32];//This variable holds the number of devices for each platform, hopefully it won't be more than 32 per platform
    cl_uint num_devices;//This number will hold the number of devices on this machine
    char deviceName[1024];//This string will hold the devices name
    cl_uint numberOfCores;//This variable holds the number of cores of on a device
    cl_long amountOfMemory;//This variable holds the amount of memory on a device
    cl_uint clockFreq;//This variable holds the clock frequency of a device
    cl_ulong maxAlocatableMem;//This variable holds the maximum allocatable memory
    cl_ulong cacheSize;
    cl_device_mem_cache_type globalCacheType;//
    cl_device_local_mem_type localMemType;//
    cl_device_type devType;//
    cl_ulong localMem;//This variable holds local memory for a device
    cl_bool available;//This variable holds if the device is available
    size_t maxWorkGroupSize;//Maximum number of work-items in a work-group executing a kernel using the data parallel execution model.
    size_t maxItemSize[3];//Maximum number of work-items that can be specified in each dimension of the work-group to clEnqueueNDRangeKernel.


    //Get the number of platforms
    clGetPlatformIDs (32, platforms, &num_platforms);
	printf("HARDWARE INFOMATION OF THIS EXPERIMENT \n");
    printf("\nNumber of platforms:%u\n\n", num_platforms);

    //This is a loop for platforms
    for(i = 0; i < num_platforms; i++) {
        printf("Platform:%u\n\n", i);
        clGetPlatformInfo (platforms[i], CL_PLATFORM_VENDOR, sizeof(vendor), vendor, NULL);
        printf("Platform Vendor:%s\n", vendor);

        clGetDeviceIDs (platforms[i], CL_DEVICE_TYPE_ALL, sizeof(devices), devices, &num_devices);
        printf("Number of devices:%u\n\n", num_devices);
        //This is a loop for devices
        for(j = 0; j < num_devices; j++) {
            //Scan in device information
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, sizeof(vendor), vendor, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(numberOfCores), &numberOfCores, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(amountOfMemory), &amountOfMemory, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clockFreq), &clockFreq, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(maxAlocatableMem), &maxAlocatableMem, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(localMem), &localMem, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_AVAILABLE, sizeof(available), &available, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cacheSize), &cacheSize, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(maxWorkGroupSize), &maxWorkGroupSize, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(maxItemSize), &maxItemSize, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(globalCacheType), &globalCacheType, NULL);//
            clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_TYPE, sizeof(localMemType), &localMemType, NULL);//

			
			
            //Print out device information
            printf("\tDevice: %u\n", j);
            printf("Name:%s \n", deviceName);
            printf("Vendor:%s\n", vendor);
            printf("Available:%s\n", available ? "Yes" : "No");
            printf("Compute Units:%u\n", numberOfCores);
            printf("Clock Frequency:%u mHz\n", clockFreq);
            printf("Global Memory:%0.00f mb\n", (double)amountOfMemory/1048576);
            printf("Max Allocateable Memory:\t%0.00f mb\n", (double)maxAlocatableMem/1048576);
            if(localMemType == CL_LOCAL){
				printf("Local MEM Type = CL_LOCAL \n");
			}
			else if(localMemType == CL_GLOBAL){
				printf("Local MEM Type = CL_GLOBAL \n");
			}
            printf("Local Memory:%u kb\n", (unsigned int)localMem/1024);
            
             if(globalCacheType == CL_NONE ){
				printf("Global Cache Type = CL_NONE \n");
			}
			else if(globalCacheType == CL_READ_ONLY_CACHE){
				printf("Global Cache Type = CL_READ_ONLY_CACHE \n");
			}
			else if(globalCacheType == CL_READ_WRITE_CACHE){
				printf("Global Cache Type = CL_READ_WRITE_CACHE \n");
			}
            printf("Global Mem Cache Size:%u kb\n", (unsigned int)cacheSize/1024);
            printf("MAX Group Size:%d \n", maxWorkGroupSize);
            printf("MAX Item Size [0]:%d \n", maxItemSize[0]);
            printf("MAX Item Size [1]:%d \n", maxItemSize[1]);
            printf("MAX Item Size [2]:%d \n\n", maxItemSize[2]);
        }
    }
    return 0;
}
