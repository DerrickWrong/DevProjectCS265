#include "gtest\gtest.h"
#include "Request.h"
#include "FileAccessor.h"
#include <string>
#include <stdio.h>

#include <map>
#include <functional>

#include "boost\filesystem.hpp"

int *generateFile(int size, int val){

	int *ptr = new int[size];

	for (int i = 0; i < size; i++){
		ptr[i] = i + val;
	}

	return ptr;
}
  
Request<int, int>* &generateArrFile(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(0L, i + offset, i);
		ptr[i] = temp;
	}

	return ptr;
} 

Request<int, int>* &generateArrReadFile(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(0L, i + offset, RequestType::QUERY);
		ptr[i] = temp;
	}

	return ptr;
}


TEST(IOTEST, testReadWrite_100k){

	std::string baseFolder = "D:\\LSM\\ReadSmallFileTest";
	std::string filename = "0_1000-1";

	int dataSize = 1000000;
	FileAccessor<int, int> fa(baseFolder);

	//generate insert requests
	Request<int, int> *dummy = generateArrFile(dataSize, 0);
	
	for (int i = 0; i < dataSize; i++){
		EXPECT_EQ(dummy[i].getValue(), i);
	}

	fa.writeFile(filename, dummy, dataSize);

	delete dummy;

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> ctree(cmp);

	fa.readFile(filename, ctree);

	for (int i = 0; i < dataSize; i++){
		EXPECT_EQ(1, ctree.count(i));
	}

	//clean up
	boost::filesystem::path p("D:\\LSM\\ReadSmallFileTest\\0_1000-1");
	boost::filesystem::remove_all(p);

}

TEST(IOTest, testWrite_1024){
	 
	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1024-1";
	int dataSize = 1024;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete dummy;
 
}

TEST(IOTest, testWrite_2048){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_2048-2";
	int dataSize = 2048;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete dummy;

}

TEST(IOTest, testWrite_4096){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_4096-3";
	int dataSize = 4096;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete dummy;

}

TEST(IOTest, testWrite_102400){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_102400-5";
	int dataSize = 102400;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete dummy;

}

TEST(IOTest, testWrite_1024000){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1024000-10";
	int dataSize = 1024000;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete dummy;

}
 
/********************************************************READ TEST ********************************************/
TEST(IOTest, testRead_1024){
	  
	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1024-1";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 1024); 
}

TEST(IOTest, testRead_2048){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_2048-2";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 2048);
}

TEST(IOTest, testRead_4096){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_4096-3";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 4096);
}

TEST(IOTest, testRead_102400){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_102400-5";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 102400);
}

TEST(IOTest, testRead_1024000){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1024000-10";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 1024000);
}

