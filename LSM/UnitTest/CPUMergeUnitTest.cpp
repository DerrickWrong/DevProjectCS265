#include "gtest\gtest.h" 
#include "Request.h"  
#include "Merger.h"
#include <vector>

int *generateCPU(int size, int val){

	int *ptr = new int[size];

	for (int i = 0; i < size; i++){
		ptr[i] = i + val;
	}

	return ptr;
}

Request<int, int> *generateArrCPU(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(0L, i + offset, i);
		ptr[i] = temp;
	}

	return ptr;
} 

TEST(CPUTest, Merge1024Test){

	int size = 1024;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}
 
TEST(CPUTest, Merge2048Test){

	int size = 2048;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(CPUTest, Merge4096Test){

	int size = 4096;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(CPUTest, Merge102400Test){

	int size = 102400;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}


TEST(CPUTest, Merge204800Test){

	int size = 204800;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(CPUTest, Merge409600Test){

	int size = 409600;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(CPUTest, Merge1024000Test){

	int size = 1024000;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(CPUTest, Merge2048000Test){

	int size = 2048000;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(CPUTest, Merge4096000Test){

	int size = 4096000;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(CPUTest, Merge10240000Test){

	int size = 10240000;

	Request<int, int> *arrA = generateArrCPU(size, 10);
	Request<int, int> *arrB = generateArrCPU(size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeCPU(arrA, arrB, size, sol);

	delete arrA;
	delete arrB;
	delete sol;
}
