#include "CudaDevice.h"   
#include "cuda_runtime.h"  

template<typename T, typename P> CudaDevice<T, P>::CudaDevice(){
}
  

template<typename T, typename P> bool CudaDevice<T, P>::isCudaAvailable(){

	int numDevice;
	cudaGetDeviceCount(&numDevice);

	return (numDevice > 0);
}

/*
* Recursion of binary device search
*/
template<typename T, typename P> __device__  void DBsearch(Request<T, P> *arrA, int startPos, int endPos, Request<T, P> &value, int &idx){

	if (startPos == endPos){
		idx = startPos;
	}
	else{
		int mid = (endPos + startPos) / 2;

		T Akey = arrA[mid].getKey();

		if (Akey > value.getKey()){
			DBsearch<T, P>(arrA, startPos, mid - 1, value, idx);
		}
		else if (Akey < value.getKey()){
			DBsearch<T, P>(arrA, mid + 1, endPos, value, idx);
		}
		else{
			idx = mid;
		}
	}
	
}

/*
* Merge B into A by compute where B's position in respect to A
*
* arrSize is always - 1024 * n where n is greater than 1
*/

template<typename T, typename P> __global__ void merge(Request<T, P> *arrA, Request<T, P> *arrB, int *arrIdx, int arrSize){

	int tid = blockIdx.x * blockDim.x + threadIdx.x;

	if (tid < arrSize){

		int work = arrSize / 1024;

		int currWorkIdx = tid * work;

		//using binary search to find the index of A
		int Apos = 0;
		DBsearch<T, P>(arrA, 0, arrSize, arrB[currWorkIdx], Apos);

		arrIdx[currWorkIdx] = arrIdx[currWorkIdx] + Apos;
		currWorkIdx = currWorkIdx + 1;
		work = work - 1;

		//process thru all the works
		while (work > 0){
			DBsearch<T, P>(arrA, Apos, arrSize, arrB[currWorkIdx], Apos);

			arrIdx[currWorkIdx] = arrIdx[currWorkIdx] + Apos;
			currWorkIdx = currWorkIdx + 1;

			work = work - 1;
		}
	}
}

template<typename T, typename P> void CudaDevice<T, P>::mergeKernel(Request<T, P> *arrayA, Request<T, P> *arrayB, int *indices, int size){

	int *idx_d;
	Request<T, P> *d_arrayA, *d_arrayB;
	int sizeOfArr = size * sizeof(Request<T, P>);
	 
	//allocate memory
	cudaMalloc(&d_arrayA, sizeOfArr);
	cudaMalloc(&d_arrayB, sizeOfArr);
	cudaMalloc(&idx_d, (size * sizeof(int)));

	//copy data from host to device
	cudaMemcpy(d_arrayA, arrayA, sizeOfArr, cudaMemcpyHostToDevice);
	cudaMemcpy(d_arrayB, arrayB, sizeOfArr, cudaMemcpyHostToDevice);
	cudaMemcpy(idx_d, indices, size * sizeof(int), cudaMemcpyHostToDevice);

	//invoke kernel
	merge<T, P> <<<32, 32>>> (d_arrayA, d_arrayB, idx_d, size);

	//move data back to host
	cudaMemcpy(arrayA, d_arrayA, sizeOfArr, cudaMemcpyDeviceToHost);
	cudaMemcpy(arrayB, d_arrayB, sizeOfArr, cudaMemcpyDeviceToHost);
	cudaMemcpy(indices, idx_d, (size * sizeof(int)), cudaMemcpyDeviceToHost);

	//free device memory
	cudaFree(d_arrayA);
	cudaFree(d_arrayB);
	cudaFree(idx_d);
}
