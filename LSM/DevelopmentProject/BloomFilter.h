#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <string>
#include <vector>
#include <tuple> 
#include <map>
#include <queue>
#include <functional>
#include <fstream>

/*
* Comparator ensure the map is sorted from smaller range to large range
*/
template<class T>
class comparator {
	
public:
	bool operator()(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs) const {
		if (lhs.second < rhs.second) {
			return true;
		}
		else if (lhs.second == rhs.second) {
			return (lhs.first > rhs.first);
		}
		else {
			return false;
		}
	}

};

/*
* BloomFilter keeps track of the files containing data and tells the
* user if a key value pair may be in that file.
*/
template<class T>
class BloomFilter { 

private: 
	std::map<std::pair<T, T>, std::string, comparator<T>> fileMap;
	std::map<std::string, long> fileSizes;
public:
	BloomFilter() {};

	/*
	* get file map by reference
	*/
	std::map<std::pair<T, T>, std::string, comparator<T>>& getFileMap() {
		
		return this->fileMap;

	};

	/*
	* Get file size
	*/
	std::map<std::string, long>& getFileSizeMap(){
		return this->fileSizes;
	};


	/*
	* update file map file into file queue
	*/
	void update(T lower, T upper, std::string path) {

		std::pair<T, T> p = std::make_pair(lower, upper);
		this->fileMap[p] = path;
		 
		std::ifstream myfile(path, std::ios::in | std::ios::binary | std::ios::ate);
		
		if (myfile.is_open()){
			this->fileSizes[path] = (long)myfile.tellg();
			myfile.close();
		} 
		 
	}

	/*
	* Remove file from file queue
	*/
	void remove(T lower, T upper) 
	{
		std::pair<T, T> p = std::make_pair(lower, upper);
		std::string file = this->fileMap[p];
		this->fileSizes.erase(file);
		this->fileMap.erase(p);
	}

};

#endif // !BLOOMFILTER_H
