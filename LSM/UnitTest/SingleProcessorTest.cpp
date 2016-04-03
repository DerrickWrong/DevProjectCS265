#include "gtest\gtest.h"
#include "Processor.h"
#include "RangePredicate.h"
#include <time.h>

#include "Utils.h"

#include <map>
#include <functional>

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

TEST(singleProcessor, bahTest){

	auto cmp = [](const int& a, const int& b) { return a < b; };
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> map(cmp);

	for (int i = 0; i < 1000; i++){
		
		Request<int, int> req(10L, i, i);
		map.insert(std::make_pair(i, req));
	}

	Request<int, int> *p = nullptr;
	int size = 0;

	Utils<int, int>::createArray(map, p, size);

	for (int i = 0; i < 1000; i++){
		
		ASSERT_EQ(p[i].getKey(), i);

	}

	ASSERT_EQ(size, 1000);

}
 
TEST(SingleProcessor, simpleFilterTest){

	//process only requests between 1m to 2m
	RangePredicate<int> *rangePred;
	rangePred = new RangePredicate<int>(0, 2048);

	Processor<int, int> processor(1024, 2, "D:\\LSM\\1m_2m", MergeType::DEVICE, rangePred);

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
	 
	//ASSERT_EQ(processor.getWork().size(), 0);
	ASSERT_EQ(processor.getQueryWork().size(), 0);

	//free resource
	delete insertReqs;   

	//boost::filesystem::path p("D:\\LSM\\1m_2m\\0_2047-2");
	//boost::filesystem::remove_all(p);

} 