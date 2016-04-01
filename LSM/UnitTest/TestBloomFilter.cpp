#include "gtest\gtest.h"
#include "BloomFilter.h"
#include <map>
#include <functional>
#include <vector>
#include <string> 

#include <iostream>
#include "boost\filesystem.hpp"

using namespace std;
using namespace boost::filesystem;


TEST(BLOOMTEST, TestFolder){

	std::string p = "D:\\LSM\\1m_2m";

	boost::filesystem::path folderPath(p);

	BloomFilter<int> bFilter(p);

	bool folderExists = boost::filesystem::exists(folderPath);

	ASSERT_EQ(folderExists, true);

	ASSERT_EQ(bFilter.getFileMap().size() == 1, true);

}


