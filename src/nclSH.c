/*
 *		Pattern Matching script with Set Horspool algorithm.
 *		This is coded by the good looking fork Peng Wu.
 * 		It under the terms of the GNU General Public License.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char * string;//Define the string type

//Load the haystack from file into memory
char* LoadHaystackDataFromFile(char *fileName) {
    FILE *filePtr;
    filePtr = fopen(fileName, "r");
    if(!filePtr) {
        fprintf(stderr, "Can not open the text file!\n");
    }
    fseek(filePtr, 0 , SEEK_END);
    int fileSize = ftell(filePtr);
    rewind(filePtr);
    char *haystack;
    haystack = (char*)malloc(sizeof(char)*fileSize);
    int haystackLen;
    haystackLen = fread(haystack, 1, fileSize, filePtr);
    if(haystackLen != fileSize) {
        fprintf(stderr, "Reading error");
    }
    fclose(filePtr);

    return haystack;
}

int* SetHorspoolPrecomputation(string *needles, int characterSetSize, int shortestNeedleLen, int needleNum) { 
    int *skipTable = (int*)malloc(sizeof(int)*characterSetSize);
    int i;
    for(i=0; i<characterSetSize; i++) {
        skipTable[i] = shortestNeedleLen;
    }
    for(i=0; i<needleNum; i++) {
        char *needlePtr = needles[i];
        int needleLen = strlen(needlePtr);
        int j;
        for(j=0; j<needleLen-1; j++) {
            int shift = needleLen-j-1;
            if(shift <= shortestNeedleLen) {
                if(skipTable[(int)needlePtr[j]] > shift) {
                    skipTable[(int)needlePtr[j]] = shift;
                }
            }
        }
    }

    return skipTable;
}


int* SetHorspoolMatch(char *haystack, string *needles, int *skipTable, int shortestNeedleLen, int needleNum) {
    int *count = (int*)malloc(sizeof(int)*needleNum);
    int haystackLen = strlen(haystack);
    int *last = (int*)malloc(sizeof(int)*needleNum);
    int i;
    for(i=0; i<needleNum; i++) { //Load each last pos counter number into last array
        last[i] = strlen(needles[i]) - 1;
        count[i] = 0;
    }
    int j=0;
    int chk =haystackLen-shortestNeedleLen;
    while(j<= chk ) {
        int k;
        int shift = shortestNeedleLen;
        for(k=0; k<needleNum; k++) {
            char *needle = needles[k];
            int lastPosofThisNeedle = last[k];
            int scanPosofThisNeedle = last[k];
            while(scanPosofThisNeedle>=0 && needle[scanPosofThisNeedle]==haystack[scanPosofThisNeedle+j]) {
                scanPosofThisNeedle--;
            }
            if(scanPosofThisNeedle<0) {
                count[k]++;
            }
            if(shift > skipTable[(int)haystack[j+lastPosofThisNeedle]]) {
                shift = skipTable[(int)haystack[j+lastPosofThisNeedle]];
            }
        }
        j+= shift;
    }
    free(last);
    
    return count;
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

void PrintResult(string *needles, int *count, int needleNum) {
    int i;
    for(i=0; i<needleNum; i++) {
        printf("CPU Non OpenCL found %d * \"%s\" in the text!!!\n",count[i],needles[i]);
    }
}


int main(int argc, char * argv[]) {
	if(argc < 3){
		printf("wrong option... \n");
		return 0;
	}
	int i;
	int needleNum = argc -2;//Calculate the number of needle
    string needles[needleNum];
    for(i=0;i<needleNum;i++){
		needles[i] = argv[i+1];
	}							
    char *fileName = argv[argc-1];// The text file's name
    int *count;
    int characterSetSize = 128;//ASCii set
    int *skipTable;
    
    int shortestNeedleLen = findShortestNeedle(needles,needleNum);//Find the shortest needle length
    skipTable = SetHorspoolPrecomputation(needles, characterSetSize, shortestNeedleLen, needleNum);
    char *haystack = LoadHaystackDataFromFile(fileName); //Load the file into memory
    count = SetHorspoolMatch(haystack, needles, skipTable, shortestNeedleLen, needleNum);//Do the matching
    PrintResult(needles, count, needleNum);//Print the result

    free(haystack); //Free the pointer in heap
    free(skipTable);
    free(count);
    
    return 0;
}
