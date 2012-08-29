/*
 *		QS algorithm
 * 		CPU with OpenCL
 * 		Local Mem
 * 		Local Counter
 *		Peng Wu.
*/
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* LoadKernelSrcFromFile(char *fileName) {
    FILE *filePtr;
    filePtr = fopen(fileName, "r");
    if(!filePtr) {
        fprintf(stderr, "can not open the text file!\n");
    }
    fseek(filePtr, 0 , SEEK_END);
    int fileSize = ftell(filePtr);
    rewind(filePtr);
    char *kernelSrc = (char*)calloc(fileSize, sizeof(char));
    int textLength;
    textLength = fread(kernelSrc, sizeof(char), fileSize, filePtr);
    if(textLength != fileSize) {
        fprintf(stderr, "reading error");
    }
    fclose(filePtr);

    return kernelSrc;
}

char* LoadHaystackDataFromFile(char *fileName) {
    FILE *filePtr;
    filePtr = fopen(fileName, "r");
    if(!filePtr) {
        fprintf(stderr, "can not open the text file!\n");
    }
    fseek(filePtr, 0 , SEEK_END);
    int fileSize = ftell(filePtr);
    rewind(filePtr);
    char *haystackData = (char*)calloc(fileSize, sizeof(char));
    int textLength;
    textLength = fread(haystackData, sizeof(char), fileSize, filePtr);
    if(textLength != fileSize) {
        fprintf(stderr, "reading error");
    }
    fclose(filePtr);

    return haystackData;
}

//Print the result
void PrintTheResults(int *res) {
	int finalCounter = 0;
	int i;
	for(i=0;i<4;i++){
		finalCounter += res[i];
	}
	printf("CPU OpenCL found matched needles = %d \n",finalCounter);
}

int* QSPrecomputation(char *needle, int characterSetSize) {
    int needleLen = strlen(needle);
    int *skipTable;
    skipTable = (int*)malloc(sizeof(int)*characterSetSize);
    int i;
    for(i=0; i<characterSetSize; i++) {
        skipTable[i] = needleLen + 1;//Different from BMH, cuz it may be able to skip the next one of the window
    }
    for(i=0; i<needleLen; i++) { //Different from BMH
        skipTable[(int)needle[i]] = needleLen-i;//This is the skip table.
    }
    return skipTable;
}

int main(int argc, char ** argv) {
	if(argc != 3){
		printf("wrong option... \n");
		return 0;
	}
    char *needleData = argv[1];
    int needleLen = strlen(needleData);
    int charsPerItem;
    int characterSetSize = 128;//ASCii set
    int *skipTable; // skipTable
    char *needle = &needleData[0];
    skipTable = QSPrecomputation(needle, characterSetSize); 

    //~ char *kernelFile = "./kernel/QS_CPU_LocalCounter_OpenCL_Kernel.cl";//the kernel
    char *kernelFile = "./kernel/QS_CPU_LocalCounter_OpenCL_Kernel.cl";//the kernel
    char *kernelSrc = LoadKernelSrcFromFile(kernelFile);
    char *textFile = argv[2];
    char *haystackData = LoadHaystackDataFromFile(textFile);
    int haystackLen = strlen(haystackData);


    cl_int err;
    cl_platform_id platform;
    cl_device_id device;

    //Find a platform
    err = clGetPlatformIDs(1, &platform, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find a platform! \n");
    }
    
    //Set up a device
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find a device! \n");
    }
    
    //Create context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a context! \n");
    }
    
    //Create command queue
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a  queue! \n");
    }
    
    //Create the programm object
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&kernelSrc, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant build the program! \n");
    }
    
    //Build the programm executable
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("cant build the programm exe! \n");
    }
    
    //Create the kernel
    cl_kernel kernel = clCreateKernel(program, "QSMatch", &err);
    if(err != CL_SUCCESS) {
        printf("cant create the kernel! \n");
    }
    
    //Create the haystack buffer
    cl_mem haystackBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, haystackLen, haystackData, &err);
    if(err != CL_SUCCESS) {
        printf("couldn't create the haystackBuffer \n");
    }
    
    //Create the results buffer
    int res[4] = {0};
    cl_mem resBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(res), res, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the resBuffer");
	}
	
    //Create the needleBuffer
    cl_mem needleBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char)*needleLen, needleData, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the needleBuffer \n");
	}
	
    //Create the skipTableBuffer
    cl_mem skipTableBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*characterSetSize, skipTable, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the skipTableBuffer \n");
	}
	
	//Determine global size and local size
    size_t localSize;
    size_t globalSize;
    
    localSize = 64;
    globalSize = 64*4;
    charsPerItem = haystackLen/globalSize + 1;//Add 1 it important otherwise some patterns will be lost
    
    //Set the kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &haystackBuffer);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &needleBuffer);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &skipTableBuffer);
    err |= clSetKernelArg(kernel, 3, sizeof(int)*1, NULL);
    err |= clSetKernelArg(kernel, 4, sizeof(needleLen), &needleLen);
    err |= clSetKernelArg(kernel, 5, sizeof(charsPerItem), &charsPerItem);
    err |= clSetKernelArg(kernel, 6, sizeof(cl_mem), &resBuffer);
    if(err != CL_SUCCESS) {
        printf("couldn't set the kernel arguments \n");
    }
    
    //Execut the kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
    //~ err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("kernel could not be executed... \n");
    } else {
        printf("kernel has been executed successfully! \n");
    }
    
    //Finish the queue list
    clFinish(queue);

    //Copy the results
    clEnqueueReadBuffer(queue, resBuffer, CL_TRUE, 0, sizeof(res), res, 0, NULL, NULL);
    PrintTheResults(res);
    
    //Clean up
    free(skipTable); //Free the skipTable
    free(kernelSrc);
    free(haystackData);
    clReleaseMemObject(haystackBuffer);
    clReleaseMemObject(needleBuffer);
    clReleaseMemObject(skipTableBuffer);
    clReleaseMemObject(resBuffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}

