#include "gtest\gtest.h"
#include "ThreadPool.h"
#include "RangePredicate.h"
#include "Processor.h" 
#include "Request.h"
#include <deque>


TEST(TestProcessor, TestQuery) {

	//create object
	Request<int, int> *mockRequest = new Request<int, int>(1L, 2, RequestType::QUERY);
	Processor<int, int> processor(new RangePredicate<int>(1, 10));

	processor.consume(*mockRequest);

	processor.execute();

	std::deque<Request<int, int>> d = processor.getQueryWork();

	ASSERT_EQ(mockRequest->getKey(), d.front().getKey());

	delete mockRequest;
}