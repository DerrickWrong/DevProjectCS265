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
		this->range = new RangePredicate<int>(0, 5000000);
		this->processor = new Processor<int, int>(5000000, 1, "D:\\LSM\\LoadTest", MergeType::DEVICE, this->range);

		Request<int, int> *ptr;

		generateInsertRequest(1000000, 0, ptr);

		for (int i = 0; i < 1000000; i++){
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

	EXPECT_EQ(1000000, C0->size());
	 
}

TEST_F(InMemory, ReadTest){

	Request<int, int> *readRequests;

	generateReadRequest(1000000, 0, readRequests);

	for (int i = 0; i < 1000000; i++){
		this->processor->consume(readRequests[i]);
	}

	delete readRequests;

	this->processor->execute();
	   
}

TEST_F(InMemory, ReadUpdateTest){

	Request<int, int> *readRequests, *insertRequests;

	generateReadRequest(1000000, 0, readRequests);
	generateInsertRequest(1000000, 500000, insertRequests);

	for (int i = 0; i < 1000000; i++){
		this->processor->consume(readRequests[i]);
		this->processor->consume(insertRequests[i]);
	}

	delete readRequests;
	delete insertRequests;

	this->processor->execute();
	 
}
