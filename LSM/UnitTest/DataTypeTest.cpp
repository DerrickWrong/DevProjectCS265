#include "gtest\gtest.h"
#include "Request.h"
#include "RangePredicate.h"
#include "Processor.h"
#include <deque>

void generateInsertDoubleRequest(int size, double offset, Request<double, double>* &ptr){

	ptr = (Request<double, double>*)malloc(sizeof(Request<double, double>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){

		Request<double, double> temp(timestamp, i + offset, i);
		ptr[i] = temp;
	}

}

void generateReadDoubleRequest(int size, double offset, Request<double, double>* &ptr){

	ptr = (Request<double, double>*)malloc(sizeof(Request<double, double>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		Request<double, double> temp(timestamp, i + offset, RequestType::QUERY);
		ptr[i] = temp;
	}
}

void generateInsertLongRequest(int size, long offset, Request<long, long>* &ptr){

	ptr = (Request<long, long>*)malloc(sizeof(Request<long, long>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){

		Request<long, long> temp(timestamp, i + offset, i);
		ptr[i] = temp;
	}

}

void generateReadLongRequest(int size, long offset, Request<long, long>* &ptr){

	ptr = (Request<long, long>*)malloc(sizeof(Request<long, long>) * size);

	long timestamp = std::time(0);

	for (int i = 0; i < size; i++){
		Request<long, long> temp(timestamp, i + offset, RequestType::QUERY);
		ptr[i] = temp;
	}
}


TEST(DoubleTest, insertTest){

	//create requests
	Request<double, double>* dReq;
	generateInsertDoubleRequest(10000, 2000.0, dReq);

	//create predicate
	RangePredicate<double> *rangePred;
	rangePred = new RangePredicate<double>(2000.0, 12000.0);

	Processor<double, double> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//consume requests
	for (int i = 0; i < 10000; i++){
	 
		processor.consume(dReq[i]);
	}

	processor.execute();

	delete [] dReq;
}

TEST(DoubleTest, readTest){

	//create requests
	Request<double, double>* dReq;
	generateReadDoubleRequest(5000, 2000.0, dReq);

	//create predicate
	RangePredicate<double> *rangePred;
	rangePred = new RangePredicate<double>(2000.0, 12000.0);

	Processor<double, double> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	for (int i = 0; i < 5000; i++){
		processor.consume(dReq[i]);
	}

	processor.execute();

	std::deque<Request<double, double>> results = processor.getQueryWork();

	EXPECT_EQ(5000, results.size());

	for (int i = 0; i < 5000; i++){
	
		EXPECT_EQ(results.at(i).getKey() - 2000.0, results.at(i).getValue());
	}

	delete [] dReq;

	//delete saved files
	boost::filesystem::path p("D:\\LSM\\MixLoad\\2000_11999-9");
	boost::filesystem::remove_all(p);
}

TEST(LongTest, insertTest){

	//create requests
	Request<long, long>* dReq;
	generateInsertLongRequest(10000, 2000L, dReq);

	//create predicate
	RangePredicate<long> *rangePred;
	rangePred = new RangePredicate<long>(2000L, 12000);

	Processor<long, long> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	//consume requests
	for (int i = 0; i < 10000; i++){

		processor.consume(dReq[i]);
	}

	processor.execute();

	delete [] dReq;
}

TEST(LongTest, readTest){

	//create requests
	Request<long, long>* dReq;
	generateReadLongRequest(5000, 2000L, dReq);

	//create predicate
	RangePredicate<long> *rangePred;
	rangePred = new RangePredicate<long>(0, 10000.0);

	Processor<long, long> processor(1024, 1, "D:\\LSM\\MixLoad", MergeType::DEVICE, rangePred);

	for (int i = 0; i < 5000; i++){
		processor.consume(dReq[i]);
	}

	processor.execute();

	std::deque<Request<long, long>> results = processor.getQueryWork();

	EXPECT_EQ(5000, results.size());

	for (int i = 0; i < 5000; i++){

		EXPECT_EQ(results.at(i).getKey() - 2000L, results.at(i).getValue());
	}

	delete[] dReq;

	//delete saved files
	boost::filesystem::path p("D:\\LSM\\MixLoad\\2000_11999-9");
	boost::filesystem::remove_all(p);
}