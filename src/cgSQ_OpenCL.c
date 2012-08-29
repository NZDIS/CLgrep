/*
 *		Pattern Matching script with Set Sunday algorithm.
 * 		It uses both CPU and GPU.
 *		This is coded by the good looking fork Peng Wu.
 * 		It under the terms of the GNU General Public License.
*/
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char * string;//Define the string type

char* LoadKernelSrcFromFile(char *fileName) {
    FILE *filePtr;
    filePtr = fopen(fileName, "r");
    if(!filePtr) {
        fprintf(stderr, "can not open the kernel text file! \n");
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
        fprintf(stderr, "can not open the haystack text file! \n");
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
void PrintTheResults(int *res, string *needles, int needleNum, int groupNum) {
    int i;
    int sum[needleNum];
    for(i=0;i<needleNum;i++){
		sum[i] = 0;
	}
	i = 0;
    while(i<needleNum*groupNum) {
        //~ printf("res[%d] =  %d \n", i, res[i]);
        sum[i%needleNum] += res[i];
        i++;
    }
	i = 0;
	while(i<needleNum){
		printf("matched %s =  %d \n", needles[i], sum[i]);
		i++;
	}
}


//Print the debug info 
void PrintTheDebugInfo(char *debug, int infoLen){
	int i=0;
	while(i<infoLen) {
        printf("debug[%d] =  %c \n", i, debug[i]);
        i++;
    }
}

int* SetSundayPrecomputation(string *needles, int characterSetSize, int shortestNeedleLen, int needleNum) { 
    int *skipTable = (int*)malloc(sizeof(int)*characterSetSize);
    int i;
    for(i=0; i<characterSetSize; i++) {
        skipTable[i] = shortestNeedleLen + 1;//Different
    }
    for(i=0; i<needleNum; i++) {
        char *needlePtr = needles[i];
        int needleLen = strlen(needlePtr);
        int j;
        for(j=0; j<needleLen; j++) {
            int shift = needleLen-j;//Different HS minus 1
            if(shift < shortestNeedleLen + 1) {//Not sure this method actually increase the performance or not
                if(skipTable[(int)needlePtr[j]] > shift) {
                    skipTable[(int)needlePtr[j]] = shift;
                }
            }
        }
    }

    return skipTable;
}

int findShortestNeedle(string *needles,int needleNum) {
    char *needlePtr = needles[0];
    int shortestLen = strlen(needlePtr);
    int i;
    for(i=0; i<needleNum; i++) {
        int needleLen = strlen(needles[i]);
        if(needleLen<shortestLen) {
            shortestLen = needleLen;
        }
    }
    
    return shortestLen;
}

int findLongestNeedle(string *needles,int needleNum) {
    char *needlePtr = needles[0];
    int longestLen = strlen(needlePtr);
    int i;
    for(i=0; i<needleNum; i++) {
        int needleLen = strlen(needles[i]);
        if(needleLen>longestLen) {
            longestLen = needleLen;
        }
    }
    return longestLen;
}

char* ConcatenateNeedlesIntoOneDataBlock(string *needles, int needleNum){
	int i;
	int lenOfNeedles = 0;
	for(i=0;i<needleNum;i++){
		lenOfNeedles += strlen(needles[i]);
	}
	//printf("the lenOfNeedles = %d \n", lenOfNeedles);
	char *needlesData = (char*)malloc(sizeof(char)*lenOfNeedles);
	for(i=0;i<needleNum;i++){
		if(i == 0){
			strcpy(needlesData,needles[0]);
		}
		else{
			strcat(needlesData, needles[i]);
		}
	}
	
	return needlesData;
}

int main(int argc, char * argv[]) {
	if(argc < 3){
		printf("wrong option... \n");
		return 0;
	}
	int t;// Reusing t can improve the performance somehow
	int needleNum = argc - 2;//Calculate the number of needle
    string needles[needleNum]; 
    for(t=0;t<needleNum;t++){
		needles[t] = argv[t+1];
	} 
	int cpuNeedleNum = needleNum*0.5;
	int gpuNeedleNum = needleNum - cpuNeedleNum;
	if(cpuNeedleNum + gpuNeedleNum == needleNum){
		printf("the needles number is right \n");
	}
	string cpuNeedles[cpuNeedleNum];
	string gpuNeedles[gpuNeedleNum];
	for(t=0;t<cpuNeedleNum;t++){
		cpuNeedles[t] = needles[t];
	}
	for(t=0;t<gpuNeedleNum;t++){
		gpuNeedles[t] = needles[t+cpuNeedleNum];
	}
	
    //Concatenating the needles into one
    char *cpuNeedlesData = ConcatenateNeedlesIntoOneDataBlock(cpuNeedles, cpuNeedleNum);
    printf("cpuNeedlesData = %s \n", cpuNeedlesData);
    char *gpuNeedlesData = ConcatenateNeedlesIntoOneDataBlock(gpuNeedles, gpuNeedleNum);
    printf("gpuNeedlesData = %s \n", gpuNeedlesData);
    int cpuLenOfneedlesData = strlen(cpuNeedlesData);
    int gpuLenOfneedlesData = strlen(gpuNeedlesData);
    int cpuShortestNeedleLen = findShortestNeedle(cpuNeedles,cpuNeedleNum);
    int gpuShortestNeedleLen = findShortestNeedle(gpuNeedles,gpuNeedleNum);
    int cpuLongestNeedleLen = findLongestNeedle(cpuNeedles,cpuNeedleNum);
    int gpuLongestNeedleLen = findLongestNeedle(gpuNeedles,gpuNeedleNum);
   
    int characterSetSize = 128;//ASCii set
    //~ int characterSetSize = 256; //UTF-8
    int *cpuSkipTable; //CPU skipTable
    int *gpuSkipTable; //GPU skipTable
    cpuSkipTable = SetSundayPrecomputation(cpuNeedles, characterSetSize, cpuShortestNeedleLen, cpuNeedleNum);
    gpuSkipTable = SetSundayPrecomputation(gpuNeedles, characterSetSize, gpuShortestNeedleLen, gpuNeedleNum);
    
    //Calculating the lastPosOfEachNeedle array and lenOfEachNeedle array
    int cpuLastPosOfEachNeedle[cpuNeedleNum];
    int gpuLastPosOfEachNeedle[gpuNeedleNum];
    int cpuLenOfEachNeedle[cpuNeedleNum]; 
    int gpuLenOfEachNeedle[gpuNeedleNum]; 
    for(t=0;t<cpuNeedleNum;t++){
		cpuLenOfEachNeedle[t] = strlen(cpuNeedles[t]); 
		cpuLastPosOfEachNeedle[t] = strlen(cpuNeedles[t]) - 1;
	}
	for(t=0;t<gpuNeedleNum;t++){
		gpuLenOfEachNeedle[t] = strlen(gpuNeedles[t]); 
		gpuLastPosOfEachNeedle[t] = strlen(gpuNeedles[t]) - 1;
	}

    char *cpuKernelFile = "./kernel/SQ_CPU_Kernel.cl";//The kernel file's name
    char *gpuKernelFile = "./kernel/SQ_GPU_LocalMem_OpenCL_Kernel.cl";//The kernel file's name
    char *cpuKernelSrc = LoadKernelSrcFromFile(cpuKernelFile);
    char *gpuKernelSrc = LoadKernelSrcFromFile(gpuKernelFile);
    char *textFile = argv[argc-1];// The text file's name
    char *haystackData = LoadHaystackDataFromFile(textFile);
    int haystackLen = strlen(haystackData);

    cl_int err;
    cl_platform_id platform;
    cl_device_id cpuDevice;
    cl_device_id gpuDevice;

    //Find a platform
    err = clGetPlatformIDs(1, &platform, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find a platform! \n");
    }
    
    //Set up devices
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &cpuDevice, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find a cpu device! \n");
    }
     err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &gpuDevice, NULL);
    if(err != CL_SUCCESS) {
        printf("cant find a gpu device! \n");
    }
    //Create context
    cl_context cpuContext = clCreateContext(NULL, 1, &cpuDevice, NULL, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a cpu context! \n");
    }
    cl_context gpuContext = clCreateContext(NULL, 1, &gpuDevice, NULL, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a gpu context! \n");
    }
    
    //Create command queue
    cl_command_queue cpuQueue = clCreateCommandQueue(cpuContext, cpuDevice, 0, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a cpu queue! \n");
    }
    cl_command_queue gpuQueue = clCreateCommandQueue(gpuContext, gpuDevice, 0, &err);
    if(err != CL_SUCCESS) {
        printf("cant create a gpu queue! \n");
    }
    
    //Create the programm object
    cl_program cpuProgram = clCreateProgramWithSource(cpuContext, 1, (const char**)&cpuKernelSrc, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant build the cpu program! \n");
    }
    cl_program gpuProgram = clCreateProgramWithSource(gpuContext, 1, (const char**)&gpuKernelSrc, NULL, &err);
    if(err != CL_SUCCESS) {
        printf("cant build the gpu program! \n");
    }
    
    //Build the programm executable
    err = clBuildProgram(cpuProgram, 0, NULL, NULL, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("cant build the cpu programm exe! \n");
    }
    err = clBuildProgram(gpuProgram, 0, NULL, NULL, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("cant build the gpu programm exe! \n");
    }
    
    //Create the kernel
    cl_kernel cpuKernel = clCreateKernel(cpuProgram, "SetSundayMatch", &err);
    if(err != CL_SUCCESS) {
        printf("cant create the cpu kernel! \n");
    }
    cl_kernel gpuKernel = clCreateKernel(gpuProgram, "SetSundayMatch", &err);
    if(err != CL_SUCCESS) {
        printf("cant create the gpu kernel! \n");
    }
    
    //Create the haystack buffer 
    //CPU and GPU program share the haystack buffer
    cl_mem cpuHaystackBuffer = clCreateBuffer(cpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, haystackLen, haystackData, &err);
    if(err != CL_SUCCESS) {
        printf("couldn't create the cpuHaystackData buffer \n");
    }
    cl_mem gpuHaystackBuffer = clCreateBuffer(cpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, haystackLen, haystackData, &err);
    if(err != CL_SUCCESS) {
        printf("couldn't create the gpuHaystackData buffer \n");
    }
    
    size_t cclocalSize;
    size_t gglocalSize;
    cclocalSize = 64;
    gglocalSize = 256;
    size_t cpuGlobalSize = 64*4;//CPU
    //~ size_t gpuGlobalSize = 256*5*8;//GPU
    size_t gpuGlobalSize = 256*24*8;//GPU
    //Group Number
    int cpuGroupNum = (int)cpuGlobalSize/cclocalSize;
    int gpuGroupNum = (int)gpuGlobalSize/gglocalSize;
    //CPU GPU share the charsPerItem
    int cpuCharsPerItem = haystackLen/cpuGlobalSize + 1;
    int gpuCharsPerItem = haystackLen/gpuGlobalSize + 1;
    
    //Create the results buffer
    int cpuRes[cpuNeedleNum*cpuGroupNum];
    int gpuRes[gpuNeedleNum*gpuGroupNum];
    for(t=0;t<cpuNeedleNum*cpuGroupNum;t++){
		cpuRes[t] = 0;
	}
	for(t=0;t<gpuNeedleNum*gpuGroupNum;t++){
		gpuRes[t] = 0;
	}
    cl_mem cpuResBuffer = clCreateBuffer(cpuContext, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cpuRes), cpuRes, &err);//Should be write only
    if(err != CL_SUCCESS) {
        printf("cant create the cpuResBuffer! \n");
    }
    cl_mem gpuResBuffer = clCreateBuffer(gpuContext, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(gpuRes), gpuRes, &err);//Should be write only
    if(err != CL_SUCCESS) {
        printf("cant create the gpuResBuffer! \n");
    }
    
    //Create the needlesBuffer 
    cl_mem cpuNeedlesBuffer = clCreateBuffer(cpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char)*cpuLenOfneedlesData, cpuNeedlesData, &err);//!!cpuNeedlesLen
    if(err != CL_SUCCESS) {
        printf("cant create the cpuNeedlesBuffer! \n");
    }
    cl_mem gpuNeedlesBuffer = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char)*gpuLenOfneedlesData, gpuNeedlesData, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the gpuNeedlesBuffer! \n");
    }
    
    //Create the skipTableBuffer
    //The skipTables of CPU and CPU are different
    cl_mem cpuSkipTableBuffer = clCreateBuffer(cpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*characterSetSize, cpuSkipTable, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the cpuSkipTableBuffer! \n");
    }
    cl_mem gpuSkipTableBuffer = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*characterSetSize, gpuSkipTable, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the gpuSkipTableBuffer! \n");
    }
    
    //Create the lastPosOfEachNeedleBuffer
    cl_mem cpuLastPosOfEachNeedleBuffer = clCreateBuffer(cpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*cpuNeedleNum, cpuLastPosOfEachNeedle, &err);//!!lastPosOfEachNeedle
    if(err != CL_SUCCESS) {
        printf("cant create the cpuLastPosOfEachNeedleBuffer! \n");
    }
    cl_mem gpuLastPosOfEachNeedleBuffer = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*gpuNeedleNum, gpuLastPosOfEachNeedle, &err);//!!gpuNeedleNum
    if(err != CL_SUCCESS) {
        printf("cant create the gpuLastPosOfEachNeedleBuffer! \n");
    }
    
    //Create the lenOfEachNeedleBuffer
    cl_mem cpuLenOfEachNeedleBuffer = clCreateBuffer(cpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*cpuNeedleNum, cpuLenOfEachNeedle, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the cpuLenOfEachNeedleBuffer! \n");
    }
    cl_mem gpuLenOfEachNeedleBuffer = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*gpuNeedleNum, gpuLenOfEachNeedle, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the gpuLenOfEachNeedleBuffer! \n");
    }
    
    
    
    //Set the kernel arguments for CPU
    err = clSetKernelArg(cpuKernel, 0, sizeof(cl_mem), &cpuHaystackBuffer);
    err |= clSetKernelArg(cpuKernel, 1, sizeof(cl_mem), &cpuNeedlesBuffer);
    err |= clSetKernelArg(cpuKernel, 2, sizeof(cl_mem), &cpuSkipTableBuffer);
    err |= clSetKernelArg(cpuKernel, 3, sizeof(cpuLongestNeedleLen), &cpuLongestNeedleLen);
    err |= clSetKernelArg(cpuKernel, 4, sizeof(cpuCharsPerItem), &cpuCharsPerItem);
    err |= clSetKernelArg(cpuKernel, 5, sizeof(cpuNeedleNum), &cpuNeedleNum);
    err |= clSetKernelArg(cpuKernel, 6, sizeof(int)*needleNum, NULL);//localCounter
    err |= clSetKernelArg(cpuKernel, 7, sizeof(cl_mem), &cpuLenOfEachNeedleBuffer);
    err |= clSetKernelArg(cpuKernel, 8, sizeof(cl_mem), &cpuResBuffer);
    if(err != CL_SUCCESS) {
        printf("couldn't set the cpu cpuKernel arguments \n");
    }
    
    //Set the kernel arguments for GPU
    err = clSetKernelArg(gpuKernel, 0, sizeof(cl_mem), &gpuHaystackBuffer);
    err |= clSetKernelArg(gpuKernel, 1, sizeof(cl_mem), &gpuNeedlesBuffer);
    err |= clSetKernelArg(gpuKernel, 2, sizeof(int), &gpuLenOfneedlesData);
    err |= clSetKernelArg(gpuKernel, 3, sizeof(char)*gpuLenOfneedlesData, NULL);//Local needlesData
    err |= clSetKernelArg(gpuKernel, 4, sizeof(cl_mem), &gpuSkipTableBuffer);
    err |= clSetKernelArg(gpuKernel, 5, sizeof(int)*128, NULL);//Local tempTable
    err |= clSetKernelArg(gpuKernel, 6, sizeof(int), &gpuLongestNeedleLen);
    err |= clSetKernelArg(gpuKernel, 7, sizeof(int), &gpuCharsPerItem);
    err |= clSetKernelArg(gpuKernel, 8, sizeof(int), &gpuNeedleNum);
    err |= clSetKernelArg(gpuKernel, 9, sizeof(int)*gpuNeedleNum, NULL);
    err |= clSetKernelArg(gpuKernel, 10, sizeof(cl_mem), &gpuLenOfEachNeedleBuffer);
    //~ err |= clSetKernelArg(gpuKernel, 11, sizeof(cl_mem), NULL);//localLEN
    err |= clSetKernelArg(gpuKernel, 11, sizeof(cl_mem), &gpuResBuffer);
    if(err != CL_SUCCESS) {
        printf("couldn't set the gpu kernel arguments \n");
    }
    
    //Execut the kernel
    //Exe on CPU
    err = clEnqueueNDRangeKernel(cpuQueue, cpuKernel, 1, NULL, &cpuGlobalSize, &cclocalSize, 0, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("CPU kernel could not be executed... \n");
    } else {
        printf("CPU kernel has been executed successfully! \n");
    }
    //Exe on GPU
    err = clEnqueueNDRangeKernel(gpuQueue, gpuKernel, 1, NULL, &gpuGlobalSize, &gglocalSize, 0, NULL, NULL);
    if(err != CL_SUCCESS) {
        printf("gPU kernel could not be executed... \n");
    } else {
        printf("gPU kernel has been executed successfully! \n");
    }
  
    //Finish the queue list
    clFinish(gpuQueue);
    clFinish(cpuQueue);
    
    //Copy the results
    clEnqueueReadBuffer(cpuQueue, cpuResBuffer, CL_TRUE, 0, sizeof(cpuRes), cpuRes, 0, NULL, NULL);
    clEnqueueReadBuffer(gpuQueue, gpuResBuffer, CL_TRUE, 0, sizeof(gpuRes), gpuRes, 0, NULL, NULL);
    PrintTheResults(cpuRes, cpuNeedles, cpuNeedleNum, cpuGroupNum);
    PrintTheResults(gpuRes, gpuNeedles, gpuNeedleNum, gpuGroupNum);

    //Clean up
    free(cpuSkipTable); 
    free(gpuSkipTable); 
    free(cpuKernelSrc);
    free(gpuKernelSrc);
    free(haystackData);
    free(cpuNeedlesData);
    free(gpuNeedlesData);
    clReleaseMemObject(cpuHaystackBuffer);
    clReleaseMemObject(gpuHaystackBuffer);
    clReleaseMemObject(cpuNeedlesBuffer);
    clReleaseMemObject(gpuNeedlesBuffer);
    clReleaseMemObject(cpuSkipTableBuffer);
    clReleaseMemObject(gpuSkipTableBuffer);
    clReleaseMemObject(cpuLenOfEachNeedleBuffer);
    clReleaseMemObject(gpuLenOfEachNeedleBuffer);
    clReleaseMemObject(cpuLastPosOfEachNeedleBuffer);
    clReleaseMemObject(gpuLastPosOfEachNeedleBuffer);
    clReleaseMemObject(cpuResBuffer);
    clReleaseMemObject(gpuResBuffer);
    clReleaseKernel(cpuKernel);
    clReleaseKernel(gpuKernel);
    clReleaseProgram(cpuProgram);
    clReleaseProgram(gpuProgram);
    clReleaseCommandQueue(cpuQueue);
    clReleaseCommandQueue(gpuQueue);
    clReleaseContext(cpuContext);
    clReleaseContext(gpuContext);

    return 0;
}

