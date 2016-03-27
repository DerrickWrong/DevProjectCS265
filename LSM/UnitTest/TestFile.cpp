#include "gtest\gtest.h"
#include "Request.h"
#include "FileAccessor.h"
#include <string>
#include <stdio.h>

int *generateFile(int size, int val){

	int *ptr = new int[size];

	for (int i = 0; i < size; i++){
		ptr[i] = i + val;
	}

	return ptr;
}

Request<int, int> *generateArrFile(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(0L, i + offset, i);
		ptr[i] = temp;
	}

	return ptr;
}

/*
TEST(FileTest, testWrite){
	 
	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1000";
	int dataSize = 1024;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete dummy;
 
}


TEST(FileTest, testIO){
	  
	FILE *pfile;

	int totalSize = 4096;
	int keyValue = (sizeof(int) + sizeof(int));
	int bufferSize = totalSize / keyValue;

	char *buffer = new char[bufferSize];

	buffer[0] = 'h';
	buffer[1] = 'e';
	buffer[2] = 'l';
	buffer[3] = 'l';
	buffer[4] = 'o';
	buffer[5] = ' ';
	buffer[6] = 'w';
	buffer[7] = 'o';
	buffer[8] = 'r';
	buffer[9] = 'l';
	buffer[10] = 'd';

	pfile = fopen("D:\\LSM\\bah.bin", "w+b");

	fwrite(buffer, keyValue, bufferSize, pfile);

	fclose(pfile);

	delete buffer;

	buffer = new char[bufferSize];

	pfile = fopen("D:\\LSM\\bah.bin", "rb");

	fread(buffer, keyValue, bufferSize, pfile);

	std::cout << "buffer " << buffer[0] << std::endl;
	std::cout << "buffer " << buffer[1] << std::endl;
	std::cout << "buffer " << buffer[2] << std::endl;
	std::cout << "buffer " << buffer[3] << std::endl;
	std::cout << "buffer " << buffer[4] << std::endl;
	std::cout << "buffer " << buffer[5] << std::endl;
	std::cout << "buffer " << buffer[6] << std::endl;
	std::cout << "buffer " << buffer[7] << std::endl;
	std::cout << "buffer " << buffer[8] << std::endl;
	std::cout << "buffer " << buffer[9] << std::endl;
	std::cout << "buffer " << buffer[10] << std::endl; 

	delete buffer;
}
*/