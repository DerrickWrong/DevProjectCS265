#include "gtest\gtest.h"
#include "BloomFilter.h"

#include <vector>
#include <string>
  
TEST(TestBloomfilter, test) {
	
	BloomFilter<int> filter;
	
	std::string f0_10 = "f0_10";
	std::string f10_20 = "f10_20"; 
	
	std::string f0_30 = "f0_30";

	filter.update(0, 10, f0_10);
	filter.update(10, 20, f10_20); 
	
	filter.update(0, 30, f0_30);

	std::map<std::pair<int, int>, std::string, comparator<int>> fileMap = filter.getMap();
	 
	std::map<std::pair<int, int>, std::string, comparator<int>>::iterator it;

	std::vector<std::string> f;

	//loop the map to find 
	for (it = fileMap.begin(); it != fileMap.end(); it++) {

		std::pair<int, int> p = it->first;
		
		if (p.first < 8 && 8 <= p.second) {
			f.push_back(it->second);
		}

	}

	//assert
	ASSERT_EQ(2, f.size());
	ASSERT_EQ(f0_10, f.at(0));
	ASSERT_EQ(f0_30, f.at(1));

	fileMap.clear();
	f.clear();

	filter.remove(10, 20);

	fileMap = filter.getMap();

	//loop the map to find 
	for (it = fileMap.begin(); it != fileMap.end(); it++) {

		std::pair<int, int> p = it->first;

		if (p.first < 11 && 11 <= p.second) {
			f.push_back(it->second);
		} 
	}

	ASSERT_EQ(1, f.size());
	ASSERT_EQ(f0_30, f.at(0));

}