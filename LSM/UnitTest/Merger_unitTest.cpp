#include "gtest\gtest.h"
#include "Merger.h"
#include <string>

TEST(TestMerger, testCreateDirectory) {
	
	std::string folder = "D:\\LSM\\0_100";

	Merger<int, int> merger(folder);
	std::wstring stemp = std::wstring(folder.begin(), folder.end());
	LPCWSTR sw = stemp.c_str();
	
	DWORD dwAttrib = GetFileAttributes(sw);
	
	bool fileExist = (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
 
	ASSERT_EQ(true, fileExist);
 
	RemoveDirectory(sw);
}


TEST(TestMerger, testCreateFile) {
	


}