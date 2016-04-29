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

TEST(IOTest, testWrite_1024){
	 
	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1024-1";
	int dataSize = 1024;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;
 
}

TEST(IOTest, testWrite_2048){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_2048-2";
	int dataSize = 2048;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_4096){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_4096-3";
	int dataSize = 4096;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_8192){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_8192-4";
	int dataSize = 8192;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_16384){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_16384-5";
	int dataSize = 16384;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_32768){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_32768-6";
	int dataSize = 32768;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_65536){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_65536-7";
	int dataSize = 65536;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_131072){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_131072-8";
	int dataSize = 131072;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_262144){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_262144-9";
	int dataSize = 262144;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_524288){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_524288-10";
	int dataSize = 524288;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_1048576){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1048576-11";
	int dataSize = 1048576;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_2097152){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_2097152-12";
	int dataSize = 2097152;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_4194304){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_4194304-13";
	int dataSize = 4194304;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

}

TEST(IOTest, testWrite_8388608){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_8388608-14";
	int dataSize = 8388608;

	FileAccessor<int, int> fa(baseFolder);

	Request<int, int> *dummy = generateArrFile(dataSize, 0);

	fa.writeFile(filename, dummy, dataSize);

	delete [] dummy;

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

TEST(IOTest, testRead_8192){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_8192-4";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 8192);
}

TEST(IOTest, testRead_16384){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_16384-5";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 16384);
}

TEST(IOTest, testRead_32768){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_32768-6";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 32768);
}

TEST(IOTest, testRead_65536){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_65536-7";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 65536);
}

TEST(IOTest, testRead_131072){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_131072-8";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 131072);
}

TEST(IOTest, testRead_262144){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_262144-9";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 262144);
}

TEST(IOTest, testRead_524288){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_524288-10";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 524288);
}

TEST(IOTest, testRead_1048576){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_1048576-11";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 1048576);
}

TEST(IOTest, testRead_2097152){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_2097152-12";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 2097152);
}

TEST(IOTest, testRead_4194304){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_4194304-13";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 4194304);
}

TEST(IOTest, testRead_8388608){

	std::string baseFolder = "D:\\LSM";
	std::string filename = "0_8388608-14";
	FileAccessor<int, int> fa(baseFolder);

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> m(cmp);

	fa.readFile(filename, m);
	ASSERT_EQ(m.size(), 8388608);
}

