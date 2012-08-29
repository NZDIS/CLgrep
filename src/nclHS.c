/*
 *		Pattern Matching script with Horspool algorithm.
 *		This is coded by the good looking fork Peng Wu.
 * 		It under the terms of the GNU General Public License.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int* HorspoolPrecomputation(char *needle, int characterSetSize) {
    int needleLen = strlen(needle);
    int *skipTable;
    skipTable = (int*)malloc(sizeof(int)*characterSetSize);
    int i;
    for(i=0; i<characterSetSize; i++) {
        skipTable[i] = needleLen;
    }
    int loopCount = needleLen -1;
    for(i=0; i<loopCount; i++) {
        skipTable[(int)needle[i]] = needleLen-1-i;
        //printf("skip value of %c = %d \n", needle[i], skipTable[(int)needle[i]]);
    }
    return skipTable;
}

int HorspoolMatch(char *haystack, char *needle, int *skipTable) {
    int i = 0;
    int count = 0;
    int needleLen = strlen(needle);
    int haystackLen = strlen(haystack);
    int loopCount = haystackLen-needleLen;
    int last = needleLen-1;
    int scan;
    while(i<=loopCount ) {
        scan = last;
        while(scan>=0 && needle[scan]==haystack[i+scan]) {
            scan--;
        }
        if(scan<0) {
            count++;
        }
        int jump = skipTable[(int)haystack[i+last]];
        if(jump > 1){
			i += jump;
		}
		else{
			i++;
		}
        
    }

    return count;
}



int main(int argc, char ** argv) {
    if(argc != 3){
		printf("wrong option... \n");
		return 0;
	}
    char *fileName = argv[2];
    char *needle = argv[1];
    int characterSetSize = 128;//ASCii set
    int *skipTable;
    int count = 0;
    skipTable = HorspoolPrecomputation(needle, characterSetSize);
    char *haystack = LoadHaystackDataFromFile(fileName); //Load the file into memory
    count = HorspoolMatch(haystack,needle,skipTable);
    printf("non CL %d matched needles was found in the text!\n",count);
    free(haystack); //Free the pointer in heap
    free(skipTable);

    return 0;
}
