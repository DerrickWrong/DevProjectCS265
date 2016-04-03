#ifndef CUDADEVICEDLL_EXPORTS
#define CUDADEVICEDLL_EXPORTS 

#include <map> 
#include "Request.h"
#include <cstdlib>

template<typename T, typename P>
class __declspec(dllexport)  CudaDevice{
 
public:
	CudaDevice();
	static bool isCudaAvailable();
	~CudaDevice(){};

	/*
	* CUDA Interface Function
	*/  
	void mergeKernel(Request<T, P> *arrayA, Request<T, P> *arrayB, int *indices, int size);
}; 

template class __declspec(dllexport) CudaDevice<int, int>;
template class __declspec(dllexport) CudaDevice<float, float>;

#endif