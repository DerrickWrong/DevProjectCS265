#include <iostream>
#include "CudaDevice.h"
#include "Request.h"

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

int main(){

	CudaDevice<int, int> cuDev;
	int size = 1024;

	Request<int, int> *arrA = generateArr(size, 10);
	Request<int, int> *arrB = generateArr(size, 0);
	int *idx = generate(size, 1);

	for (int i = 0; i < 3; i++){
		std::cout << "A: " << arrA[i].getKey() << " B: " << arrB[i].getKey() << " index " << idx[i] << std::endl;
	}

	std::cout << "Running kernel " << std::endl;

	cuDev.mergeKernel(arrB, arrA, idx, size);

	std::cout << "Done running kernel " << std::endl;

	for (int i = 0; i < 3; i++){
		std::cout << "A: " << arrA[i].getKey() << " B: " << arrB[i].getKey() << " index " << idx[i] << std::endl;
	}

	delete idx;
	delete arrA;
	delete arrB;

	system("pause");
	return 0;
}