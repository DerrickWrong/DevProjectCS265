#include "CudaMerger.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>

__global__ void CMerge(int* arrA, int* arrB, int* arrIdx, int idxSize){

	int tid = blockIdx.x * blockDim.x + threadIdx.x;

	if (tid < idxSize){
		
		arrIdx[tid] = arrIdx[tid] + 20;

	}

}


CudaMerger::CudaMerger(){

}

void CudaMerger::merge(int* &arrA, int lenA, int* &arrB, int lenB){
	
	int *d_idx, *idx, *d_arrA, *d_arrB;
	int sizeAarr = lenA * sizeof(int);
	int sizeBarr = lenB * sizeof(int);

	//initialize idx
	idx = new int[lenB];

	for (int i = 0; i < lenB; i++){
		idx[i] = i;
	}

	//allocate data
	cudaMalloc(&d_idx, sizeBarr);
	cudaMalloc(&d_arrA, sizeAarr);
	cudaMalloc(&d_arrB, sizeBarr);

	//copy data from host to device
	cudaMemcpy(d_arrA, arrA, sizeAarr, cudaMemcpyHostToDevice);
	cudaMemcpy(d_arrB, arrB, sizeBarr, cudaMemcpyHostToDevice);
	cudaMemcpy(d_idx, idx, sizeBarr, cudaMemcpyHostToDevice);

	//invoke kernel
	CMerge<<<32, 32>>>(d_arrA, d_arrB, d_idx, lenB);

	//move data back to host
	cudaMemcpy(idx, d_idx, sizeBarr, cudaMemcpyDeviceToHost);

	for (int i = 0; i < lenB; i++){
		std::cout << "index: " << idx[i] << std::endl;
	}

	//free resources
	cudaFree(d_arrA);
	cudaFree(d_arrB);
	cudaFree(d_idx);
	delete idx;
}