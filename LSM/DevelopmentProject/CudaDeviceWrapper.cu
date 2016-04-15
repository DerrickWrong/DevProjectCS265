#include "CudaDevice.h"   
#include "cuda_runtime.h"  

#include <iostream>

template<typename T, typename P> CudaDevice<T, P>::CudaDevice(){
}
  

template<typename T, typename P> bool CudaDevice<T, P>::isCudaAvailable(){

	int numDevice;
	cudaGetDeviceCount(&numDevice);

	return (numDevice > 0);
}



template<typename T, typename P> __device__  void DBsearch(Request<T, P>* arrA, int startPos, int endPos, T key, int &idx){

	if (startPos == endPos){
		idx = startPos;
	}
	else{
		int mid = (endPos + startPos) / 2;

		T Akey = arrA[mid].getKey();

		if (Akey > key){
			DBsearch<T, P>(arrA, startPos, mid - 1, key, idx);
		}
		else if (Akey < key){
			DBsearch<T, P>(arrA, mid + 1, endPos, key, idx);
		}
		else{
			idx = mid;
		}
	}
	
}

 
template<typename T, typename P> __global__ void merge(Request<T, P>* arrA, Request<T, P>* arrB, int* arrIdx, int arrSize){

	int tid = blockIdx.x * blockDim.x + threadIdx.x;

	if (tid < arrSize){

		int offset = 1024;
		int workIdx = tid;
		
		int Apos;
		T key;

		//move by thread idx
		while (workIdx < arrSize){
			
			key = arrB[workIdx].getKey();
			 
			DBsearch<T, P>(arrA, 0, arrSize, key, Apos);

			arrIdx[workIdx] += Apos;
  
			//increment w 
			workIdx = workIdx + offset;
		} 
	}
	 
}

template<typename T, typename P> void CudaDevice<T, P>::mergeKernel(Request<T, P>* &arrayA, int arrASize, Request<T, P>* &arrayB, int* &indices, int size){
	
	int *idx_d;
	Request<T, P> *d_arrayA = 0;
	Request<T, P> *d_arrayB = 0;
	int sizeOfArrA = arrASize * sizeof(Request<T, P>);
	int sizeOfArrB = size * sizeof(Request<T, P>);
	
	//allocate memory
	cudaMalloc(&d_arrayA, sizeOfArrA);
	cudaMalloc(&d_arrayB, sizeOfArrB);
	cudaMalloc(&idx_d, (size * sizeof(int)));
	
	
	//copy data from host to device
	cudaMemcpy(d_arrayA, arrayA, sizeOfArrA, cudaMemcpyHostToDevice);
	cudaMemcpy(d_arrayB, arrayB, sizeOfArrB, cudaMemcpyHostToDevice);
	cudaMemcpy(idx_d, indices, size * sizeof(int), cudaMemcpyHostToDevice);

	//invoke kernel 
	merge<T, P> << <32, 32 >> >(d_arrayA, d_arrayB, idx_d, arrASize);

	//move data back to host
	cudaMemcpy(indices, idx_d, (size * sizeof(int)), cudaMemcpyDeviceToHost);
	
	//free device memory
	cudaFree(d_arrayA);
	cudaFree(d_arrayB);
	cudaFree(idx_d);
	
}
