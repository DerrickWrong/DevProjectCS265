#include "gtest\gtest.h" 
#include "Request.h"
#include "CudaDevice.h" 
#include "Merger.h"

int *generate(int size, int val){

	int *ptr = new int[size];

	for (int i = 0; i < size; i++){
		ptr[i] = i + val;
	}

	return ptr;
}

Request<int, int> *generateArr(long timestamp, int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(timestamp, i + offset, i);
		ptr[i] = temp;
	}

	return ptr;
}

TEST(GPUTest, Merge1024Test){
	 
	int size = 1024;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge1024_TestTwo){

	int size = 1025;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge2048Test){

	int size = 2048;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge4096Test){

	int size = 4096;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge102400Test){

	int size = 102400;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge204800Test){

	int size = 204800;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge409600Test){

	int size = 409600;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge1024000Test){

	int size = 1024000;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge2048000Test){

	int size = 2048000;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge4096000Test){

	int size = 4096000;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge10240000Test){

	int size = 10240000;

	Request<int, int> *arrA = generateArr(10L, size, 10);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}