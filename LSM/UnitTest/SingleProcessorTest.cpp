#include "gtest\gtest.h"
#include "Processor.h"
#include "RangePredicate.h"
#include <time.h>

#include "Utils.h"

#include <map>
#include <functional>

#include "Request.h"
#include "FileAccessor.h"



Request<int, int>* &generateInsertRequest(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		
		Request<int, int> temp(timestamp, i + offset, i);
		ptr[i] = temp;
	}

	return ptr;
}

Request<int, int>* &generateReadRequest(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(timestamp, i + offset, RequestType::QUERY);
		ptr[i] = temp;
	}

	return ptr;
}


class SingleProcessorFixture : public ::testing::Test{

protected:
	SingleProcessorFixture(){
	}

	virtual void SetUp() {
		Request<int, int> *insertReqs;
		int size = 1024;

		insertReqs = generateInsertRequest(size, 0);

		FileAccessor<int, int> fa("D:\\LSM\\MixLoad");

		fa.writeFile("0_1024-1", insertReqs, size);

		delete insertReqs;
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

};

TEST(SingleProcessor, ReadLoadTest){

	//process only requests between 1m to 2m
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 2048);

	Processor<int, int> processor(1024, 2, "D:\\LSM\\ReadTest", MergeType::DEVICE, rangePred);

	//create read requests
	Request<int, int> *readReqs;
	int size = 1024;
	readReqs = generateReadRequest(size, 1024);

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

	//process only requests between 1m to 2m
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 2048);

	Processor<int, int> processor(1024, 2, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create insert requests
	Request<int, int> *insertReqs;
	int size = 1024;
	insertReqs = generateInsertRequest(size, 1024);

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
	  
} 

TEST(SingleProcessorTest, mixLoadTest){
	
	//process only requests between 1m to 2m
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(2048, 4096);

	Processor<int, int> processor(1024, 2, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create insert requests for 1024+
	Request<int, int> *insertReqs;
	int readSize = 1024;
	int writeSize = 2048;
	insertReqs = generateInsertRequest(writeSize, 2048);

	for (int i = 0; i < writeSize; i++){
		processor.consume(insertReqs[i]);
	}

	//create read requests
	Request<int, int> *readReqs;
	readReqs = generateReadRequest(readSize, 0);

	//submit them to the processor
	for (int i = 0; i < readSize; i++){
		processor.consume(readReqs[i]);
	}
	 
	processor.execute();
 
	ASSERT_EQ(processor.getWork().size(), 0);

	//it gets filtered
	ASSERT_EQ(processor.getQueryWork().size(), 0);

	//free resource
	delete insertReqs;
	delete readReqs;

	boost::filesystem::path p("D:\\LSM\\MixLoad\\0_4095-4");
	boost::filesystem::remove_all(p);
	
}
