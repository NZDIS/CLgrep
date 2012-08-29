/*
 *		QS algorithm
 * 		CPU and GPU with OpenCL
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
    int ccfileSize = fileSize*0.5;
    //~ int ccfileSize = fileSize - ggfileSize;
    rewind(filePtr);
    //Split the file then I need overlaping!
    //~ char *gghaystackData = (char*)calloc(ggfileSize, sizeof(char));
    char *cchaystackData = (char*)calloc(ccfileSize, sizeof(char));
    //~ int textLength;
    //~ fread(gghaystackData, sizeof(char), ggfileSize, filePtr);
    fread(cchaystackData, sizeof(char), ccfileSize, filePtr);
    //~ if(textLength != fileSize) {
        //~ fprintf(stderr, "reading error");
    //~ }
    fclose(filePtr);
	//pointer array can solve this issue
	return cchaystackData;
}

//Print the result
void PrintTheResults(int *ccres, int *ggres, int gpuGroupNum) {
	int i;
	int ggCounter = 0;
	int ccCounter = ccres[0]+ccres[1]+ccres[2]+ccres[3];
	for(i=0;i<gpuGroupNum;i++){
		ggCounter += ggres[i];
	}
	int finRes = ccCounter+ggCounter;
    printf("CPU&GPUfound %d matched pattern... \n", finRes);
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
    int cccharsPerItem;
    int ggcharsPerItem;
    int characterSetSize = 128;//ASCii set
    int *skipTable; // skipTable
    char *needle = &needleData[0];//!!!Pass it in a wrong way
    skipTable = QSPrecomputation(needle, characterSetSize); 

    char *cckernelFile = "./kernel/QS_CPU_LocalCounter_OpenCL_Kernel.cl";//the kernel for CPU
    char *ggkernelFile = "./kernel/QS_GPU_LocalMem_LocalCounter_OpenCL_Kernel.cl";//the kernel for GPU
    char *cckernelSrc = LoadKernelSrcFromFile(cckernelFile);
    char *ggkernelSrc = LoadKernelSrcFromFile(ggkernelFile);
    char *fileName = argv[2];
    
    //Load the haystacks from file
    FILE *filePtr;
    filePtr = fopen(fileName, "r");
    if(!filePtr) {
        fprintf(stderr, "can not open the text file!\n");
    }
    fseek(filePtr, 0 , SEEK_END);
    int fileSize = ftell(filePtr);
    int ggfileSize = fileSize*0.5;
    int ccfileSize = fileSize - ggfileSize;
    rewind(filePtr);
    //Split the file then I need overlaping!
    char *gghaystackData = (char*)calloc(ggfileSize, sizeof(char));
    char *cchaystackData = (char*)calloc(ccfileSize, sizeof(char));
    int textLength;
    textLength = fread(gghaystackData, sizeof(char), ggfileSize, filePtr);
    textLength += fread(cchaystackData, sizeof(char), ccfileSize, filePtr);
    if(textLength != fileSize) {
        fprintf(stderr, "reading error");
    }
    fclose(filePtr);
    //~ char *cchaystackData = LoadHaystackDataFromFile(fileName);
    int cchaystackLen = strlen(cchaystackData);
    int gghaystackLen = strlen(gghaystackData);


    cl_int err;
    cl_platform_id platform;
    cl_device_id cdevice;
    cl_device_id ggdevice;

    //Find a platform
    err = clGetPlatformIDs(1, &platform, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find a platform! \n");
    }
    
    //Set up devices
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &cdevice, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find CPU! \n");
    }
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &ggdevice, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find GPU! \n");
    }
    
    //Create context
    cl_context ccontext = clCreateContext(NULL, 1, &cdevice, NULL, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a ccontext! \n");
    }
    cl_context ggcontext = clCreateContext(NULL, 1, &ggdevice, NULL, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a ggcontext! \n");
    }
    
    //Create command queues
    cl_command_queue cqueue = clCreateCommandQueue(ccontext, cdevice, 0, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a  cqueue! \n");
    }
    cl_command_queue ggqueue = clCreateCommandQueue(ggcontext, ggdevice, 0, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a  ggqueue! \n");
    }

    
    //Create the programm object
    cl_program cprogram = clCreateProgramWithSource(ccontext, 1, (const char**)&cckernelSrc, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant build the program! \n");
    }
    cl_program ggprogram = clCreateProgramWithSource(ggcontext, 1, (const char**)&ggkernelSrc, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant build the program! \n");
    }

    
    //Build the programm executable
    err = clBuildProgram(cprogram, 0, NULL, NULL, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("cant build the cprogramm exe! \n");
    }
    err = clBuildProgram(ggprogram, 0, NULL, NULL, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("cant build the ggprogramm exe! \n");
    }

    
    //Create the kernel
    cl_kernel ckernel = clCreateKernel(cprogram, "QSMatch", &err);
    if(err != CL_SUCCESS) {
        printf("cant create the ckernel! \n");
    }
    cl_kernel ggkernel = clCreateKernel(ggprogram, "QSMatch", &err);
    if(err != CL_SUCCESS) {
        printf("cant create the ggkernel! \n");
    }

    
    //Create the haystack buffer
    cl_mem cchaystackBuffer = clCreateBuffer(ccontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, cchaystackLen, cchaystackData, &err);
    if(err != CL_SUCCESS) {
        printf("couldn't create the cchaystackBuffer \n");
    }
    cl_mem gghaystackBuffer = clCreateBuffer(ggcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, gghaystackLen, gghaystackData, &err);
    if(err != CL_SUCCESS) {
        printf("couldn't create the gghaystackBuffer \n");
    }

	//Determine global size and local size
    size_t cclocalSize = 64;
    size_t gglocalSize = 256;
    size_t ccGlobalSize = 64*4;
    //~ size_t ggGlobalSize = 256*5*8;
    size_t ggGlobalSize = 256*24*8;
    int gpuGroupNum = (int)ggGlobalSize/gglocalSize;
    cccharsPerItem = cchaystackLen/ccGlobalSize + 1;//Add 1 it important otherwise some patterns will be lost
    ggcharsPerItem = cchaystackLen/ggGlobalSize + 1;
  
    //Create the results buffer
    int ccres[4] = {0};
    int ggres[gpuGroupNum];
    int k;
    for(k=0;k<gpuGroupNum;k++){
		ggres[k] = 0;
	}
    
    cl_mem ccresBuffer = clCreateBuffer(ccontext, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(ccres), ccres, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the ccresBuffer");
	}
	cl_mem ggresBuffer = clCreateBuffer(ggcontext, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(ggres), ggres, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the ggresBuffer");
	}
	
    //Create the needleBuffer
    cl_mem ccneedleBuffer = clCreateBuffer(ccontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char)*needleLen, needleData, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the ccneedleBuffer \n");
	}
	cl_mem ggneedleBuffer = clCreateBuffer(ggcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char)*needleLen, needleData, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the ggneedleBuffer \n");
	}
	
    //Create the skipTableBuffer
    cl_mem ccskipTableBuffer = clCreateBuffer(ccontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*characterSetSize, skipTable, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the ccskipTableBuffer \n");
	}
	cl_mem ggskipTableBuffer = clCreateBuffer(ggcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*characterSetSize, skipTable, &err);
    if(err != CL_SUCCESS){
		printf("couldn't create the ggskipTableBuffer \n");
	}
	
    //Set the kernel arguments for CPU
    err = clSetKernelArg(ckernel, 0, sizeof(cl_mem), &cchaystackBuffer);
    err |= clSetKernelArg(ckernel, 1, sizeof(cl_mem), &ccneedleBuffer);
    err |= clSetKernelArg(ckernel, 2, sizeof(cl_mem), &ccskipTableBuffer);
    err |= clSetKernelArg(ckernel, 3, sizeof(int)*1, NULL);
    err |= clSetKernelArg(ckernel, 4, sizeof(needleLen), &needleLen);
    err |= clSetKernelArg(ckernel, 5, sizeof(cccharsPerItem), &cccharsPerItem);
    err |= clSetKernelArg(ckernel, 6, sizeof(cl_mem), &ccresBuffer);
    if(err != CL_SUCCESS) {
        printf("couldn't set the ckernel arguments \n");
    }
    
    //Set the kernel arguments for GPU
    err = clSetKernelArg(ggkernel, 0, sizeof(cl_mem), &gghaystackBuffer);
    err |= clSetKernelArg(ggkernel, 1, sizeof(cl_mem), &ggneedleBuffer);
    err |= clSetKernelArg(ggkernel, 2, sizeof(char)*needleLen, NULL);
    err |= clSetKernelArg(ggkernel, 3, sizeof(cl_mem), &ggskipTableBuffer);
    err |= clSetKernelArg(ggkernel, 4, sizeof(int)*128, NULL);
    err |= clSetKernelArg(ggkernel, 5, sizeof(int)*1, NULL);
    err |= clSetKernelArg(ggkernel, 6, sizeof(needleLen), &needleLen);
    err |= clSetKernelArg(ggkernel, 7, sizeof(ggcharsPerItem), &ggcharsPerItem);
    err |= clSetKernelArg(ggkernel, 8, sizeof(cl_mem), &ggresBuffer);
    if(err != CL_SUCCESS) {
        printf("couldn't set the ggkernel arguments \n");
    }
    
    //Execut the kernel
    //On CPU
    err = clEnqueueNDRangeKernel(cqueue, ckernel, 1, NULL, &ccGlobalSize, &cclocalSize, 0, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("ckernel could not be executed... \n");
    } 
    else {
        printf("ckernel has been executed successfully! \n");
    }
      
    //~ //On GPU
    err = clEnqueueNDRangeKernel(ggqueue, ggkernel, 1, NULL, &ggGlobalSize, &gglocalSize, 0, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("ggkernel could not be executed... \n");
    } 
    else {
        printf("ggkernel has been executed successfully! \n");
    }
    
    //Finish the queue lists
    clFinish(ggqueue);
    clFinish(cqueue);
    

    //Copy the results
    clEnqueueReadBuffer(cqueue, ccresBuffer, CL_TRUE, 0, sizeof(ccres), ccres, 0, NULL, NULL);
    clEnqueueReadBuffer(ggqueue, ggresBuffer, CL_TRUE, 0, sizeof(ggres), ggres, 0, NULL, NULL);
    PrintTheResults(ccres, ggres, gpuGroupNum);
    
    //Clean up
    free(skipTable);
    free(cckernelSrc);
    free(ggkernelSrc);
    free(cchaystackData);
    free(gghaystackData);
    clReleaseMemObject(cchaystackBuffer);
    clReleaseMemObject(ccneedleBuffer);
    clReleaseMemObject(ccskipTableBuffer);
    clReleaseMemObject(ccresBuffer);
    clReleaseMemObject(gghaystackBuffer);
    clReleaseMemObject(ggneedleBuffer);
    clReleaseMemObject(ggskipTableBuffer);
    clReleaseMemObject(ggresBuffer);
    clReleaseKernel(ckernel);
    clReleaseKernel(ggkernel);
    clReleaseProgram(cprogram);
    clReleaseProgram(ggprogram);
    clReleaseCommandQueue(cqueue);
    clReleaseCommandQueue(ggqueue);
    clReleaseContext(ccontext);
    clReleaseContext(ggcontext);

    return 0;
}

