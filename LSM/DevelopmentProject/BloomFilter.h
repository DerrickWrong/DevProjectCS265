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
	std::map<int, std::string, std::function<bool(const int&, const int&)>> *files;
	std::map<std::string, std::pair<T, T>> pairMap;
	
public:
	BloomFilter(std::string folder) {
	
		auto cmp = [](const int& a, const int& b) { return a < b; };
		this->files = new std::map<int, std::string, std::function<bool(const int&, const int&)>>(cmp);

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

				this->update(upperKey, lowerKey, current_file, lvl);
			}
		}

	};

	~BloomFilter(){
		delete this->files;
	};

	/*
	* Get file size
	*/
	std::map<int, std::string, std::function<bool(const int&, const int&)>> *getLevelMap(){
		
		return this->files;
	};

	std::pair<T, T> &getPair(std::string &s){
		
		return this->pairMap.at(s);
	};


	/*
	* update file map file into file queue
	*/
	bool update(T upper, T lower, std::string path, int level) {
		
		if (this->files->count(level)){
			return false;
		}
		else{
			this->files->insert(std::make_pair(level, path));
			this->pairMap.insert(std::make_pair(path, std::make_pair(upper, lower)));
			return true;
		} 
	};

	/*
	* Remove file from file queue and disk
	*/
	void remove(int key) 
	{
		std::string file = this->files->at(key);
		this->pairMap.erase(file);
		this->files->erase(key);

		boost::filesystem::path p(file);
		boost::filesystem::remove(p);
	};

};

#endif // !BLOOMFILTER_H
