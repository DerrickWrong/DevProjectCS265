#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <string>
#include <vector>
#include <tuple> 
#include <map>
#include <queue>
#include <functional>
#include <fstream>

#include "boost\filesystem.hpp"

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
	std::string baseFolderPath;

	std::map<std::pair<T, T>, std::string, comparator<T>> fileMap;
	std::map<std::string, int> levelMap;

	
public:
	BloomFilter(std::string folder) {
	
		this->baseFolderPath = folder;

		//create directory if not exists
		boost::filesystem::path dir(folder.c_str());
		boost::filesystem::create_directories(dir);

		//list all files  
		boost::filesystem::directory_iterator end_itr;
		 
		//cycle through the directory
		for (boost::filesystem::directory_iterator itr(dir); itr != end_itr; ++itr)
		{
			if (boost::filesystem::is_regular_file(itr->path())) {
				std::string current_file = itr->path().string();
				
				//process and update to map
				int desirePos = current_file.find_last_of("\\");
				current_file = current_file.substr(desirePos + 1, current_file.length());
				int uscore = current_file.find("_"); 
				int dash = current_file.find("-");

				std::stringstream LB(current_file.substr(0, uscore));
				std::stringstream UB(current_file.substr(uscore + 1, dash));
				std::stringstream SB(current_file.substr(dash + 1, current_file.length() - 1));

				T lowerKey, upperKey;
				int lvl;

				LB >> lowerKey;
				UB >> upperKey;
				SB >> lvl;

				this->update(lowerKey, upperKey, current_file, lvl);
			}
		}

	};

	/*
	* get file map by reference
	*/
	std::map<std::pair<T, T>, std::string, comparator<T>>& getFileMap() {
		
		return this->fileMap;

	};

	/*
	* Get file size
	*/
	std::map<std::string, int>& getLevelMap(){
		
		return this->levelMap;

	};


	/*
	* update file map file into file queue
	*/
	void update(T lower, T upper, std::string path, int level) {

		std::pair<T, T> p = std::make_pair(lower, upper);
		this->fileMap[p] = path;
		this->levelMap[path] = level;

	}

	/*
	* Remove file from file queue
	*/
	void remove(T lower, T upper) 
	{
		std::pair<T, T> p = std::make_pair(lower, upper);
		std::string file = this->fileMap[p];
		this->levelMap.erase(file);
		this->fileMap.erase(p);
	}

};

#endif // !BLOOMFILTER_H
