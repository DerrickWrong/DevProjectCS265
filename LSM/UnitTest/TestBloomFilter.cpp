#include "gtest\gtest.h"
#include "BloomFilter.h"
#include <map>
#include <functional>

TEST(BLOOMTEST, TestFile){

	BloomFilter<int> bFilter;

	std::string filename = "D:\\LSM\\0_1024";

	bFilter.update(0, 1024, filename);

	std::map<std::pair<int, int>, std::string, comparator<int>> filemap = bFilter.getFileMap();

	std::map<std::string, long> sizeMap = bFilter.getFileSizeMap();

	ASSERT_EQ(filemap.size(), 1);
	ASSERT_EQ(sizeMap.size(), 1);


}
