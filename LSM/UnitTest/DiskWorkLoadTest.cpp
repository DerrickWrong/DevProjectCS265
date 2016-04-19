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