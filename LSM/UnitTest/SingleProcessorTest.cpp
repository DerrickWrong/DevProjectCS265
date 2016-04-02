#include "gtest\gtest.h"
#include "Processor.h"
#include "RangePredicate.h"
#include <time.h>

Request<int, int> *generateInsertRequest(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		
		Request<int, int> temp(timestamp, i + offset, i);
		ptr[i] = temp;
	}

	return ptr;
}

Request<int, int> *generateReadRequest(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(timestamp, i, RequestType::QUERY);
		ptr[i] = temp;
	}

	return ptr;
}

Request<int, int> *generateRemoveRequest(int size, int offset){

	Request<int, int> *ptr;
	ptr = (Request<int, int>*)malloc(sizeof(Request<int, int>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		Request<int, int> temp(timestamp, i, RequestType::REMOVE);
		ptr[i] = temp;
	}

	return ptr;
}


TEST(SingleProcessor, simpleFilterTest){

	//process only requests between 1m to 2m
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(1000000, 2000000);

	Processor<int, int> processor(1024, 2, "D:\\LSM\\1m_2m", MergeType::DEVICE, rangePred);

	//create insert requests
	Request<int, int> *insertReqs;
	int size = 10000;
	insertReqs = generateInsertRequest(size, 999000);

	//submit them to the processor
	for (int i = 0; i < size; i++){
		processor.consume(insertReqs[i]);
	}

	//validate only certain works are accepted
	ASSERT_EQ(processor.getWork().size(), 9000);

	//create read requests
	Request<int, int> *readReqs;
	int readSize = 100;
	readReqs = generateReadRequest(readSize, 1000000);

	for (int i = 0; i < readSize; i++){
		processor.consume(readReqs[i]);
	}

	//free resource
	delete insertReqs;
	delete readReqs;
}