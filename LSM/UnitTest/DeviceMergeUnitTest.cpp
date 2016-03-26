#include "gtest\gtest.h" 
#include "Request.h"
#include "CudaDevice.h" 

int *generate(int size, int val){

	int *ptr = new int[size];

	for (int i = 0; i < size; i++){
		ptr[i] = i + val;
	}

	return ptr;
}

Request<int, int> *generateArr(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(0L, i + offset, i);
		ptr[i] = temp;
	}

	return ptr;
}

TEST(GPUTest, Merge1024Test){

	CudaDevice<int, int> cuDev;
	int size = 1024;

	Request<int, int> *arrA = generateArr(size, 10);
	Request<int, int> *arrB = generateArr(size, 0);
	int *idx = generate(size, 1);

	cuDev.mergeKernel(arrB, arrA, idx, size);

	delete idx;
	delete arrA;
	delete arrB;
}

//2 times of 1024 workload
TEST(GPUTest, Merge2048Test){

	CudaDevice<int, int> cuDev;
	int size = 2048;

	Request<int, int> *arrA = generateArr(size, 10);
	Request<int, int> *arrB = generateArr(size, 0);
	int *idx = generate(size, 1);

	cuDev.mergeKernel(arrB, arrA, idx, size);

	delete idx;
	delete arrA;
	delete arrB;
}

//4 times of 1024 workload
TEST(GPUTest, Merge4096Test){

	CudaDevice<int, int> cuDev;
	int size = 4096;

	Request<int, int> *arrA = generateArr(size, 10);
	Request<int, int> *arrB = generateArr(size, 0);
	int *idx = generate(size, 1);

	cuDev.mergeKernel(arrB, arrA, idx, size);

	delete idx;
	delete arrA;
	delete arrB;
}

//10 times of 1024 workload
TEST(GPUTest, Merge102400Test){

	CudaDevice<int, int> cuDev;
	int size = 102400;

	Request<int, int> *arrA = generateArr(size, 10);
	Request<int, int> *arrB = generateArr(size, 0);
	int *idx = generate(size, 1);

	cuDev.mergeKernel(arrB, arrA, idx, size);

	delete idx;
	delete arrA;
	delete arrB;
}

//100 times of 1024 workload
TEST(GPUTest, Merge10240000Test){

	CudaDevice<int, int> cuDev;
	int size = 10240000;

	Request<int, int> *arrA = generateArr(size, 10);
	Request<int, int> *arrB = generateArr(size, 0);
	int *idx = generate(size, 1);

	cuDev.mergeKernel(arrB, arrA, idx, size);

	delete idx;
	delete arrA;
	delete arrB;
}
