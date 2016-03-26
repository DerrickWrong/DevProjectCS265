#include "gtest\gtest.h" 
#include "Request.h"  

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

	Request<int, int> *allArray = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);

	int cidx = size;

	//combine two arrays
	for (int i = 0; i < size; i++){

		allArray[i] = std::move(arrA[i]);
		allArray[cidx] = std::move(arrB[i]);
		cidx = cidx + 1;
	}

	delete arrA;
	delete arrB;

	delete allArray;
}