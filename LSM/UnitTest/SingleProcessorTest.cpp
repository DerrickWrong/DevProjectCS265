#include "gtest\gtest.h"
#include "Processor.h"
#include "RangePredicate.h"
#include <time.h>
#include <deque>

#include "Utils.h"

#include <map>
#include <functional>

#include "Request.h"
#include "FileAccessor.h"

void generateInsertRequest(int size, int offset, Request<int, int>* &ptr){

	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		
		Request<int, int> temp(timestamp, i + offset, i + offset);
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

void createFiles(std::string name, int size, int offset, std::string baseDir){

	Request<int, int> *insertReqs = NULL;

	generateInsertRequest(size, offset, insertReqs);

	FileAccessor<int, int> fa(baseDir);

	fa.writeFile(name, insertReqs, size);

	delete[] insertReqs;
}

class TradeOffStudy : public ::testing::Test{

protected:

	TradeOffStudy(){}

	virtual void SetUp() {
		//create files
		createFiles("0_1023-1", 1024, 0, "D:\\LSM\\MixLoad");
		createFiles("2048_4097-2", 2048, 2048, "D:\\LSM\\MixLoad");
		createFiles("8192_12287-4", 4096, 8192, "D:\\LSM\\MixLoad");
		createFiles("20480_28671-8", 8192, 20480, "D:\\LSM\\MixLoad");
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
		//remove files
		boost::filesystem::path p1("D:\\LSM\\MixLoad\\0_1023-1");
		boost::filesystem::path p2("D:\\LSM\\MixLoad\\2048_4097-2");
		boost::filesystem::path p3("D:\\LSM\\MixLoad\\8192_12287-4");
		boost::filesystem::path p4("D:\\LSM\\MixLoad\\20480_28671-8");

		boost::filesystem::remove_all(p1);
		boost::filesystem::remove_all(p2);
		boost::filesystem::remove_all(p3);
		boost::filesystem::remove_all(p4);
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

	std::deque<Request<int, int>> rdeque = processor.getQueryWork();

	ASSERT_EQ(processor.getQueryWork().size(), 1024);

	//validate content in value
	for (int i = 0; i < rdeque.size(); i++){
		EXPECT_EQ(i, rdeque.at(i).getValue());
	}

	//free resource
	delete [] readReqs;

}
 
TEST(SingleProcessorFixture, insertLoadTest){

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
	delete [] insertReqs;  

	//delete saved files
	boost::filesystem::path p("D:\\LSM\\MixLoad\\0_2047-2");
	boost::filesystem::remove_all(p);
	  
} 

TEST(SingleProcessorFixture, mixLoadTest){

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

	std::deque<Request<int, int>> rdeque = processor.getQueryWork();

	//it gets filtered
	ASSERT_EQ(readSize, rdeque.size());

	//validate content in value
	for (int i = 0; i < rdeque.size(); i++){
		EXPECT_EQ(i, rdeque.at(i).getValue());
	}
	//free resource
	delete [] insertReqs;
	delete [] readReqs;

	boost::filesystem::path p("D:\\LSM\\MixLoad\\0_4095-4");
	boost::filesystem::remove_all(p);
	
}

TEST(SingleProcessorFixture, RepeatedTest){
	
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

	delete [] requests;

	generateInsertRequest(size, 1024, requests);

	//repeat insert for two times
	for (int i = 0; i < size; i++){
		processor.consume(requests[i]);
	}

	delete [] requests;

	processor.execute(); 

	boost::filesystem::path p("D:\\LSM\\MixLoad\\1024_10023-1");
	boost::filesystem::remove_all(p);

}

TEST_F(TradeOffStudy, baseCase){
}

TEST_F(TradeOffStudy, TradeOff_readAbit){
 
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 30000);

	Processor<int, int> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create read requests
	Request<int, int> *ptr;
	generateReadRequest(1, 500, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	generateReadRequest(1, 3000, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	generateReadRequest(1, 9000, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	generateReadRequest(1, 25000, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	//execute
	processor.execute();

	//verify contents
	std::deque<Request<int, int>> rst = processor.getQueryWork();
	EXPECT_EQ(500, rst.at(0).getValue());
	EXPECT_EQ(3000, rst.at(1).getValue());
	EXPECT_EQ(9000, rst.at(2).getValue());
	EXPECT_EQ(25000, rst.at(3).getValue()); 
}

TEST_F(TradeOffStudy, TradeOff_readAlot){

	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 30000);

	Processor<int, int> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create read requests
	Request<int, int> *ptr;
	generateReadRequest(100, 500, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;

	generateReadRequest(100, 3000, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;

	generateReadRequest(100, 9000, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;

	generateReadRequest(100, 25000, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;

	//execute
	processor.execute();

	//verify contents
	std::deque<Request<int, int>> rst = processor.getQueryWork();
	EXPECT_EQ(400, rst.size());

}

TEST_F(TradeOffStudy, TradeOff_updateAbit){
	
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 30000);

	Processor<int, int> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create read requests
	Request<int, int> *ptr;
	generateInsertRequest(1, 500, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	generateInsertRequest(1, 3000, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	generateInsertRequest(1, 9000, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	generateInsertRequest(1, 25000, ptr);
	processor.consume(ptr[0]);
	delete[] ptr;

	processor.execute();
}


TEST_F(TradeOffStudy, TradeOff_updateAlot){

	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 30000);

	Processor<int, int> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//create read requests
	Request<int, int> *ptr;
	generateInsertRequest(100, 0, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;
	
	generateInsertRequest(100, 2048, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;
	
	generateInsertRequest(100, 8192, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;
	
	generateInsertRequest(100, 20480, ptr);
	for (int i = 0; i < 100; i++) processor.consume(ptr[i]);
	delete[] ptr;
	
	//execute
	processor.execute();

}