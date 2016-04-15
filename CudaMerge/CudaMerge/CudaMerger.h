#ifndef CUDAMERGER_H
#define CUDAMERGER_H

#include "cuda_runtime.h"

class CudaMerger{

public:
	CudaMerger();
	void merge(int* &arrA, int lenA, int* &arrB, int lenB);
};

#endif