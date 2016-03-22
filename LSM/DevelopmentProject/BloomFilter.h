#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <string>
#include <vector>
#include <tuple> 
#include <map>
#include <queue>
#include <functional>

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
public:
	BloomFilter() {};

	/*
	* get file map by reference
	*/
	std::map<std::pair<T, T>, std::string, comparator<T>>& getMap() {
		
		return this->fileMap;

	};

	/*
	* update file map file into file queue
	*/
	void update(T lower, T upper, std::string path) {

		this->fileMap[std::make_pair(lower, upper)] = path;
	}

	/*
	* Remove file from file queue
	*/
	void remove(T lower, T upper) 
	{
		this->fileMap.erase(std::make_pair(lower, upper));
	}

};

#endif // !BLOOMFILTER_H
