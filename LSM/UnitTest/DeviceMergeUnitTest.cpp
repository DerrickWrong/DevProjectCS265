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

TEST(GPUTest, mergeTest512){

	int size = 512;

	Request<int, int> *arrA = generateArr(10L, 10, 17);
	Request<int, int> *arrB = generateArr(20L, size, 0);
	Request<int, int> *sol = NULL;
	Merger<int, int> merger("D:\\LSM");

	merger.invokeGPUmerge(arrB, size, arrA, 10, sol);
	
	for (int i = 0; i < size + 10; i++){
		std::cout << i << " key " <<  sol[i].getKey() << std::endl;
	}

	delete arrA;
	delete arrB;
	delete sol;

}
/*
TEST(GPUTest, Merge1024Test){
	 
	int size = 1024;

	Request<int, int> *arrA = generateArr(10L, size, 1024);
	Request<int, int> *arrB = generateArr(20L, size, 0);
	Request<int, int> *sol = NULL;
	Merger<int, int> merger("D:\\LSM");

	merger.invokeGPUmerge(arrA, size, arrB, size, sol); 

	for (int i = 0; i < (size * 2); i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge2048Test){

	int size = 2048;

	Request<int, int> *arrA = generateArr(10L, size, size);
	Request<int, int> *arrB = generateArr(0L, size, 0);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrA, arrB, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}


TEST(GPUTest, Merge4096Test){

	int size = 4096;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge8192Test){

	int size = 8192;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge16384Test){

	int size = 16384;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge32768Test){

	int size = 32768;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge65536Test){

	int size = 65536;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge131072Test){

	int size = 131072;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge262144Test){

	int size = 262144;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge524288Test){

	int size = 524288;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge1048576Test){

	int size = 1048576;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}

TEST(GPUTest, Merge2097152Test){

	int size = 2097152;

	Request<int, int> *arrA = generateArr(10L, size, 0);
	Request<int, int> *arrB = generateArr(0L, size, size);
	Request<int, int> *sol = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size * 2);
	Merger<int, int> merger("D:\\LSM");

	merger.mergeGPU(arrB, arrA, size, sol);

	for (int i = 0; i < size * 2; i++){
		EXPECT_EQ(i, sol[i].getKey());
	}

	delete arrA;
	delete arrB;
	delete sol;
}
*/