#include "gtest\gtest.h"
#include "Processor.h"
#include "RangePredicate.h"
#include <time.h>

#include "Utils.h"

#include <map>
#include <functional>

#include "Request.h"
#include "FileAccessor.h"

void generateInsertRequest(int size, int offset, Request<int, int>* &ptr){

	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		
		Request<int, int> temp(timestamp, i + offset, i);
		ptr[i] = temp;
	}
	 
}

void generateReadRequest(int size, int offset, Request<int, int>* &ptr){

	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){ 
		Request<int, int> temp(timestamp, i + offset, RequestType::QUERY);
		ptr[i] = temp;
	}
}

class SingleProcessorFixture : public ::testing::Test{

protected:
	SingleProcessorFixture(){
	}

	virtual void SetUp() {
	  
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	void createFiles(std::string name, int size, int offset, std::string baseDir){
	
		Request<int, int> *insertReqs = NULL;

		generateInsertRequest(size, offset, insertReqs);

		FileAccessor<int, int> fa(baseDir);

		fa.writeFile(name, insertReqs, size);

		delete insertReqs;
	}

};

TEST(SingleProcessor, ReadLoadTest){

	//process only requests between 1m to 2m
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 2048);

	Processor<int, int> processor(1024, 1, "D:\\LSM\\ReadTest", MergeType::DEVICE, rangePred);

	//create read requests
	Request<int, int> *readReqs;
	int size = 1024;
	generateReadRequest(size, 0, readReqs);

	//submit them to the processor
	for (int i = 0; i < size; i++){
		processor.consume(readReqs[i]);
	}

	processor.execute();

	ASSERT_EQ(processor.getQueryWork().size(), 1024);

	//free resource
	delete readReqs;

}
 
TEST_F(SingleProcessorFixture, insertLoadTest){

	createFiles("0_1024-1", 1024, 0, "D:\\LSM\\MixLoad");

	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 2048);

	Processor<int, int> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create insert requests
	Request<int, int> *insertReqs = NULL;
	int size = 1024;
	generateInsertRequest(size, 1024, insertReqs);

	//submit them to the processor
	for (int i = 0; i < size; i++){
		processor.consume(insertReqs[i]);
	}

	//validate only certain works are accepted
	ASSERT_EQ(processor.getWork().size(), 1024);

	processor.execute();
	  
	ASSERT_EQ(processor.getWork().size(), 0);

	//free resource
	delete insertReqs;  

	boost::filesystem::path p("D:\\LSM\\MixLoad\\0_2047-2");
	boost::filesystem::remove_all(p);
	  
} 

TEST_F(SingleProcessorFixture, mixLoadTest){

	createFiles("0_2047-2", 2048, 0, "D:\\LSM\\MixLoad");

	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 4096);

	Processor<int, int> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create insert requests for 1024+
	Request<int, int> *insertReqs = NULL;
	int readSize = 1024;
	int writeSize = 2048;
	generateInsertRequest(writeSize, 2048, insertReqs);

	for (int i = 0; i < writeSize; i++){
		processor.consume(insertReqs[i]);
	}

	//create read requests
	Request<int, int> *readReqs = NULL;
	generateReadRequest(readSize, 0, readReqs);

	//submit them to the processor
	for (int i = 0; i < readSize; i++){
		processor.consume(readReqs[i]);
	}
	 
	processor.execute();
 
	ASSERT_EQ(processor.getWork().size(), 0);

	//it gets filtered
	ASSERT_EQ(processor.getQueryWork().size(), readSize);

	//free resource
	delete insertReqs;
	delete readReqs;

	boost::filesystem::path p("D:\\LSM\\MixLoad\\0_4095-4");
	boost::filesystem::remove_all(p);
	
}

TEST_F(SingleProcessorFixture, RepeatedTest){
	
	RangePredicate<int> *rangePredicate;
	rangePredicate = new RangePredicate<int>(0, 1000000);

	Processor<int, int> processor(2000, 3, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePredicate);

	//create insert requests
	Request<int, int> *requests = NULL;
	int size = 9000;
	generateInsertRequest(size, 1024, requests);

	//repeat insert for two times
	for (int i = 0; i < size; i++){
		processor.consume(requests[i]);
	}

	processor.execute(); 

	delete requests;

	boost::filesystem::path p("D:\\LSM\\MixLoad\\1024_10023-1");
	boost::filesystem::remove_all(p);

}

