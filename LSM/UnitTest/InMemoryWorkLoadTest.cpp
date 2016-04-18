#include "gtest\gtest.h"

#include <deque>
#include "Request.h"
#include "Processor.h"
#include "RangePredicate.h"

#include <functional>
#include <map>

class InMemory : public ::testing::Test{

protected:
	Processor<int, int> *processor;
	RangePredicate<int> *range;

	InMemory(){
		this->range = new RangePredicate<int>(0, 10000);
		this->processor = new Processor<int, int>(10000, 1, "D:\\LSM\\LoadTest", MergeType::DEVICE, this->range);

		Request<int, int> *ptr;

		generateInsertRequest(5000, 0, ptr);

		for (int i = 0; i < 5000; i++){
			this->processor->consume(ptr[i]);
		}

		delete ptr;

		this->processor->execute();
	}

	~InMemory(){
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

TEST_F(InMemory, UpdateTest){

	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> *C0;

	C0 = processor->getMap();

	EXPECT_EQ(5000, C0->size());
	 
}

TEST_F(InMemory, ReadTest){

	Request<int, int> *readRequests;

	generateReadRequest(5000, 0, readRequests);

	for (int i = 0; i < 5000; i++){
		this->processor->consume(readRequests[i]);
	}

	delete readRequests;

	this->processor->execute();
	 
	std::deque<Request<int, int>> buf = this->processor->getQueryWork();

	for (int i = 0; i < buf.size(); i++){
		EXPECT_EQ(i, buf[i].getValue());
	}
}

TEST_F(InMemory, ReadUpdateTest){

	Request<int, int> *readRequests, *insertRequests;

	generateReadRequest(5000, 0, readRequests);
	generateInsertRequest(5000, 2500, insertRequests);

	for (int i = 0; i < 5000; i++){
		this->processor->consume(readRequests[i]);
		this->processor->consume(insertRequests[i]);
	}

	delete readRequests;
	delete insertRequests;

	this->processor->execute();

	//validate the results
	std::deque<Request<int, int>> rbuf = this->processor->getQueryWork();
	std::deque<Request<int, int>> wbuf = this->processor->getWork();

	EXPECT_EQ(5000, rbuf.size());

	for (int i = 0; i < rbuf.size(); i++){
		EXPECT_EQ(i, rbuf[i].getValue());
	}

	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> *C0;

	C0 = processor->getMap();

	EXPECT_EQ(7500, C0->size());
	  
}
