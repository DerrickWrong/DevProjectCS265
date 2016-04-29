#include "gtest\gtest.h"

#include <deque>
#include "Request.h"
#include "Processor.h"
#include "RangePredicate.h"

#include <functional>
#include <map>

class InDisk : public ::testing::Test{

protected:
	Processor<int, int> *processor;
	RangePredicate<int> *range;

	InDisk(){
		this->range = new RangePredicate<int>(0, 1000000);
		this->processor = new Processor<int, int>(10000, 1, "D:\\LSM\\LoadTest", MergeType::DEVICE, this->range);

		Request<int, int> *ptr;

		generateInsertRequest(5000, 0, ptr);

		for (int i = 0; i < 5000; i++){
			this->processor->consume(ptr[i]);
		}

		delete ptr;

		this->processor->execute();
	}

	~InDisk(){
		delete processor;
	}

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

};

/*
* Create update to previous file on disk
*/
TEST_F(InDisk, InsertTest){

	//create insert requests
	Request<int, int> *data;

	generateInsertRequest(7500, 2500, data);

	for (int i = 0; i < 7500; i++){
		this->processor->consume(data[i]);
	}

	delete [] data;

	//save to file
	this->processor->execute();
}

TEST_F(InDisk, ReadInsertTest){

	//create insert requests
	Request<int, int> *data;

	generateInsertRequest(7500, 2500, data);

	for (int i = 0; i < 7500; i++){
		this->processor->consume(data[i]);
	}

	delete [] data;

	//generate read requests
	generateReadRequest(10000, 0, data);

	for (int i = 0; i < 10000; i++){
		this->processor->consume(data[i]);
	}

	delete [] data;

	//execute
	this->processor->execute();

	EXPECT_EQ(10000, this->processor->getQueryWork().size());

	boost::filesystem::path p("D:\\LSM\\LoadTest\\0_9999-1");
	boost::filesystem::remove_all(p);
}


TEST_F(InDisk, ReadRandomTest){

	// 10k to 20k
	Request<int, int> *data; 

	generateInsertRequest(5000, 5000, data);

	for (int i = 0; i < 5000; i++){
		processor->consume(data[i]);
	}

	delete [] data;

	//execute
	processor->execute();
	
	// 40k to 60k
	generateInsertRequest(20000, 40000, data);
	
	for (int i = 0; i < 20000; i++){
		this->processor->consume(data[i]);
	}

	//execute
	this->processor->execute();
	delete [] data;
 
	//Generate Read Requests: 0 - 1000 read requests 
	generateReadRequest(1000, 0, data);

	for (int i = 0; i < 1000; i++){
		this->processor->consume(data[i]);
	}

	delete [] data;

	//Generate read requests: 50000 - 51000 read requests
	generateReadRequest(1000, 51000, data);

	for (int i = 0; i < 1000; i++){
		this->processor->consume(data[i]);
	}

	delete [] data;

	//run the read requests
	this->processor->execute();

	EXPECT_EQ(2000, this->processor->getQueryWork().size());

	//remove old files
	boost::filesystem::path p("D:\\LSM\\LoadTest\\0_9999-1");
	boost::filesystem::remove_all(p);

	boost::filesystem::path p2("D:\\LSM\\LoadTest\\40000_59999-2");
	boost::filesystem::remove_all(p2);

}