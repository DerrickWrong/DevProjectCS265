#include "gtest\gtest.h"
#include "Merger.h"
#include <map>
#include <functional>
#include <deque>

std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> *makeMap(int size, int offset){
	
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> *m;

	auto cmp = [](const int& a, const int& b) { return a < b; };
	m = new std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>>(cmp);

	for (int i = 0; i < size; i++){
		
		Request<int, int> tmp(0L, i, i + offset);
		m->insert(std::make_pair(i, tmp));
	}

	return m;
}


void createReadRequests(std::deque<Request<int, int>> &requests, int size, int offset){

	for (int i = 0; i < size; i++){
		Request<int, int> r(0L, i + offset, RequestType::QUERY);
		requests.push_back(r);
	}

}

TEST(MergerTest, TestProcessReadQueryInMemory_size_100K_requests_10k){
	 
	std::map<int, Request<int, int>, std::function<bool(const int&, const int&)>> *mp;
	mp = makeMap(100000, 0);

	ASSERT_EQ(mp->size(), 100000);

	std::deque<Request<int, int>> requests;
	std::deque<Request<int, int>> completed;
	
	createReadRequests(requests, 10000, 2000);

	Merger<int, int>::processReadQuery(requests, completed, *mp);

	ASSERT_EQ(requests.size(), 0);
	ASSERT_EQ(completed.size(), 10000);

	delete mp;
}


TEST(MergerTest, TestProcessReadQueryDisk_query_1024){
	  
	Merger<int, int> merger(1024, 2, "D:\\LSM\\1m_2m", MergeType::ONBOARD);

	//create requests
	std::deque<Request<int, int>> requests;
	createReadRequests(requests, 1024, 0);

	merger.query(requests);

	ASSERT_EQ(requests.size(), 1024);
	ASSERT_EQ(requests.at(0).getValue(), 0);
	ASSERT_EQ(requests.at(1023).getValue(), 1023);
}