#include "gtest\gtest.h"
#include <fstream>
#include <iostream>
#include <cstring>

class BufferFixture : public ::testing::Test {

protected:

	char* buffer;
	int bufferLen = 4096;
	int idx = 0;

	BufferFixture(){
		buffer = new char[bufferLen];
	};

	~BufferFixture(){
		delete this->buffer;
	};

};

void writeData(int data, int &idx, char* buffer){

	buffer[idx] = data & 0xff;
	buffer[idx + 1] = (data >> 8) & 0xff;
	buffer[idx + 2] = (data >> 16) & 0xff;
	buffer[idx + 3] = (data >> 24) & 0xff;

	idx = idx + 4;
}

void writeData(long data, int &idx, char* buffer){
	buffer[idx] = data & 0xff;
	buffer[idx + 1] = (data >> 8) & 0xff;
	buffer[idx + 2] = (data >> 16) & 0xff;
	buffer[idx + 3] = (data >> 24) & 0xff;

	idx = idx + 4;
}

void writeData(float data, int &idx, char* buffer){
	std::memcpy(&buffer[idx], &data, sizeof(float));
	idx = idx + 4;
}
void readData(char* buffer, int &idx, int &data){

	data = (buffer[idx] & 0xff) | (buffer[idx + 1] << 8 & 0xff) | (buffer[idx] << 16 & 0xff) | (buffer[idx] << 24 & 0xff);
	idx = idx + 4;
}

void readData(char* buffer, int &idx, long &data){
	data = (buffer[idx] & 0xff) | (buffer[idx + 1] << 8 & 0xff) | (buffer[idx] << 16 & 0xff) | (buffer[idx] << 24 & 0xff);
	idx = idx + 4;
}

/******************************************
*  Integer Test
*******************************************/
TEST_F(BufferFixture, writeIntTest){

	for (int i = 0; i < 100; i++){
		writeData(i, idx, buffer);
	}

	std::ofstream file;
	file.open("D:\\Test\\bah.bin", std::ios::binary);

	file.write(buffer, sizeof(char) * idx);

	file.close();
}

TEST_F(BufferFixture, readIntTest){

	std::ifstream file("D:\\Test\\bah.bin", std::ios::in | std::ios::binary);

	file.read(buffer, bufferLen * sizeof(char));

	int data;

	for (int i = 0; i < 100; i++){
		readData(buffer, idx, data);
		EXPECT_EQ(i, data);
	}

	file.close();
}


/******************************************
*  Long Test
*******************************************/
TEST_F(BufferFixture, writeLongTest){

	for (int i = 0; i < 100; i++){
		writeData(long(i), idx, buffer);
	}

	std::ofstream file;
	file.open("D:\\Test\\bah.bin", std::ios::binary);

	file.write(buffer, sizeof(char) * idx);

	file.close();
}

TEST_F(BufferFixture, readLongTest){

	std::ifstream file("D:\\Test\\bah.bin", std::ios::in | std::ios::binary);

	file.read(buffer, bufferLen * sizeof(char));

	long data;

	for (int i = 0; i < 100; i++){
		readData(buffer, idx, data);
		EXPECT_EQ(i, data);
	}

	file.close();
}


