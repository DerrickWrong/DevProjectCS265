#include "gtest\gtest.h"
#include "BloomFilter.h"
#include <map>
#include <functional>

TEST(BLOOMTEST, TestFile){

	BloomFilter<int> bFilter;

	bFilter.update(0, 1000, "D:\\LSM\\bah.bin");

	std::map<std::pair<int, int>, std::string, comparator<int>> filemap = bFilter.getFileMap();

	std::map<std::string, long> sizeMap = bFilter.getFileSizeMap();

	ASSERT_EQ(filemap.size(), 1);
	ASSERT_EQ(sizeMap.size(), 1);

}
