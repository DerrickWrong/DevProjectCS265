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

TEST(BLOOMTEST, TestFile){

	BloomFilter<int> bFilter;

	std::string filename = "D:\\LSM\\0_1024";

	bFilter.update(0, 1024, filename);

	std::map<std::pair<int, int>, std::string, comparator<int>> filemap = bFilter.getFileMap();

	std::map<std::string, long> sizeMap = bFilter.getFileSizeMap();

	ASSERT_EQ(filemap.size(), 1);
	ASSERT_EQ(sizeMap.size(), 1);


}


TEST(BLOOMTEST, listFile){
 
	// list all files in current directory.
	//You could put any file path in here, e.g. "/home/me/mwah" to list that directory
	path p("D:\\LSM");

	directory_iterator end_itr;

	// cycle through the directory
	for (directory_iterator itr(p); itr != end_itr; ++itr)
	{
		// If it's not a directory, list it. If you want to list directories too, just remove this check.
		if (is_regular_file(itr->path())) {
			// assign current file name to current_file and echo it out to the console.
			string current_file = itr->path().string();
			cout << current_file << endl;
		}
	}

}

