/*
 *		Pattern Matching script with Set Sunday algorithm.
 * 		It is based on GPU with OpenCL.
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
	int t;//Reusing t can improve the performance somehow
	int needleNum = argc - 2;//Calculate the number of needle
    string needles[needleNum]; 
    for(t=0;t<needleNum;t++){
		needles[t] = argv[t+1];
	} 
	
    //Concatenating the needles into one
    char *needlesData = ConcatenateNeedlesIntoOneDataBlock(needles, needleNum);
    int lenOfneedlesData = strlen(needlesData);
    int shortestNeedleLen = findShortestNeedle(needles,needleNum);//Find the shortest needle length
    int longestNeedleLen = findLongestNeedle(needles,needleNum);
   
    int characterSetSize = 128;//ASCii set
    //~ int characterSetSize = 256; //UTF-8
    int *skipTable; //The skipTable
    skipTable = SetSundayPrecomputation(needles, characterSetSize, shortestNeedleLen, needleNum);
    
    //Calculating the lastPosOfEachNeedle array and lenOfEachNeedle array
    int lastPosOfEachNeedle[needleNum];
    int lenOfEachNeedle[needleNum]; 
    for(t=0;t<needleNum;t++){
		lenOfEachNeedle[t] = strlen(needles[t]); 
		lastPosOfEachNeedle[t] = strlen(needles[t]) - 1;
	}

    char *kernelFile = "./kernel/SQ_GPU_LocalMem_OpenCL_Kernel.cl";//The kernel file's name
    char *kernelSrc = LoadKernelSrcFromFile(kernelFile);
    char *textFile = argv[argc-1];// The text file's name
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
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
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
    cl_kernel kernel = clCreateKernel(program, "SetSundayMatch", &err);
    if(err != CL_SUCCESS) {
        printf("cant create the kernel! \n");
    }
    
    //Create the haystack buffer
    cl_mem haystackBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, haystackLen, haystackData, &err);
    if(err != CL_SUCCESS) {
        printf("couldn't create the haystackData buffer \n");
    }
    
    size_t localSize;
    size_t globalSize;
    localSize = 256;
    //~ globalSize = 256*5*8;
    globalSize = 256*24*8;
    int groupNum = (int)globalSize/localSize;
    int charsPerItem = haystackLen/globalSize + 1;//Add 1 it important otherwise some patterns will be lost
    
    //Create the results buffer
    int res[needleNum*groupNum];
    for(t=0;t<needleNum*groupNum;t++){
		res[t] = 0;
	}
    cl_mem resBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(res), res, &err);//Should be write only
    if(err != CL_SUCCESS) {
        printf("cant create the resBuffer! \n");
    }
    
    //Create the needlesBuffer 
    cl_mem needlesBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char)*lenOfneedlesData, needlesData, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the needlesBuffer! \n");
    }
    
    //Create the skipTableBuffer
    cl_mem skipTableBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*characterSetSize, skipTable, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the skipTableBuffer! \n");
    }
    
    //Create the lastPosOfEachNeedleBuffer
    cl_mem lastPosOfEachNeedleBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*needleNum, lastPosOfEachNeedle, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the lastPosOfEachNeedleBuffer! \n");
    }
    
    //Create the lenOfEachNeedleBuffer
    cl_mem lenOfEachNeedleBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*needleNum, lenOfEachNeedle, &err);
    if(err != CL_SUCCESS) {
        printf("cant create the lenOfEachNeedleBuffer! \n");
    }
    
    //Set the kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &haystackBuffer);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &needlesBuffer);
    err |= clSetKernelArg(kernel, 2, sizeof(int), &lenOfneedlesData);
    err |= clSetKernelArg(kernel, 3, sizeof(char)*lenOfneedlesData, NULL);//Local needlesData
    err |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &skipTableBuffer);
    err |= clSetKernelArg(kernel, 5, sizeof(int)*128, NULL);//Local tempTable
    err |= clSetKernelArg(kernel, 6, sizeof(int), &longestNeedleLen);
    err |= clSetKernelArg(kernel, 7, sizeof(int), &charsPerItem);
    err |= clSetKernelArg(kernel, 8, sizeof(int), &needleNum);
    err |= clSetKernelArg(kernel, 9, sizeof(int)*needleNum, NULL);
    err |= clSetKernelArg(kernel, 10, sizeof(cl_mem), &lenOfEachNeedleBuffer);
    //~ err |= clSetKernelArg(kernel, 11, sizeof(cl_mem), NULL);//localLEN
    err |= clSetKernelArg(kernel, 11, sizeof(cl_mem), &resBuffer);
    if(err != CL_SUCCESS) {
        printf("couldn't set the kernel arguments \n");
    }
    
    //Execut the kernel
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);//Problem with the localSize
    if(err != CL_SUCCESS) {
        printf("kernel could not be executed... \n");
    } else {
        printf("gPU kernel has been executed successfully! \n");
    }
    //Finish the queue list
    clFinish(queue);
    
    //Copy the results
    clEnqueueReadBuffer(queue, resBuffer, CL_TRUE, 0, sizeof(res), res, 0, NULL, NULL);
    PrintTheResults(res, needles, needleNum, groupNum);

    //Clean up
    free(skipTable); 
    free(kernelSrc);
    free(haystackData);
    free(needlesData);
    clReleaseMemObject(haystackBuffer);
    clReleaseMemObject(needlesBuffer);
    clReleaseMemObject(skipTableBuffer);
    clReleaseMemObject(lenOfEachNeedleBuffer);
    clReleaseMemObject(lastPosOfEachNeedleBuffer);
    clReleaseMemObject(resBuffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}

