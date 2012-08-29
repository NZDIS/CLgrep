//~ #pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
__kernel void QSMatch(
	__global const char *haystack, 
	__global const char *needle, //Global mem
	__local char *tempNeedle,//Local mem
	__global const int *skipTable, //Global mem
	__local int *tempTable,//Local mem
	__local int *localCounter,//Local mem
	int needleLen,
	int charsPerItem,
	__global int *res){
	
	//Copying
	int localIndex = get_local_id(0);
	int i;
	int privateCounter = 0;
	if(localIndex == 0){
		for(i=0;i<needleLen;i++){//Copy global needle to local mem
			tempNeedle[i]=needle[i];
		}
		localCounter[0] = 0;
		//~ for(i=0;i<128;i++){
			//~ tempTable[i] = skipTable[i];
		//~ }
	}
	if(localIndex < 128){
		tempTable[localIndex] = skipTable[localIndex];
	}
	
	//Syncing here
	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
	
	//Offseting
    int index = get_global_id(0);
    int offset = 0;//Deploying offset array buffer may be improve the performance
    if(index != 0){
		offset = (index*charsPerItem) - needleLen + 1; //Overlaping
	}
	
	//Doing
    i = 0;//Reuse loop counter
    int posPtr;
	while(i<charsPerItem){
		posPtr = 0; 
		while((posPtr<needleLen) && (tempNeedle[posPtr] == haystack[offset+i+posPtr])){//What if camparing needleLen address of mem together???
			posPtr++;
		}
		if(posPtr == needleLen ){
			privateCounter++;
		}
		if(tempTable[haystack[(offset+i+needleLen)]] > 1){
			i += tempTable[haystack[(offset+i+needleLen)]];
		}
		else{
			i++;
		}
	}
	
	//Res from private to local
	atomic_add(&localCounter[0], privateCounter);
	
	//Syncing here
	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
	
	//Last item send res from local mem to global mem
	if(localIndex == 0){
		int groupId = get_group_id(0); 
		res[groupId] = localCounter[0];		
	}
}
