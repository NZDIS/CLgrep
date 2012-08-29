#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
__kernel void QSMatch(
	__global const char *haystack, 
	__global const char *needle, //Global mem
	__global const int *skipTable, //Global mem
	__local int *localCounter,//Local mem
	int needleLen,
	int charsPerItem,
	__global int *res){
	
	//Copying
	int localIndex = get_local_id(0);//Costly, it can be put to another function maybe
	int i;
	int privateCounter = 0;
	if(localIndex == 0){
		localCounter[0] = 0;
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
    int scan;
    int lastPos = needleLen - 1;
	while(i<charsPerItem){
		scan = lastPos; 
		while((scan >= 0) && (needle[scan] == haystack[offset+i+scan])){
			scan--;
		}
		if(scan < 0 ){
			privateCounter++;
		}	
		i += skipTable[haystack[(offset+i+lastPos)]];
	}
	
	//Res from private to local
	atomic_add(&localCounter[0], privateCounter);
	
	//Syncing here
	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
	
	//Last item send res from local mem to global mem
	int groupId = get_group_id(0); 
	int myLocalSize = get_local_size(0); 
	if(localIndex == myLocalSize - 1){
		res[groupId] = localCounter[0];		
	}
}
