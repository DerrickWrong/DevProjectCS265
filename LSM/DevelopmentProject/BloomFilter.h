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
	std::map<int, std::string, std::function<bool(const int&, const int&)>> *files = nullptr; 
	std::map<std::string, std::pair<T, T>> pairMap;
	int levelRatio = 0;
	
public:
	BloomFilter(std::string folder, int level) {
	  
		this->levelRatio = level;

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
				this->update(current_file);
			}
		}

	}; 

	~BloomFilter(){};

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
	bool update(std::string path) {
		
		//process and update to map
		int desirePos = path.find_last_of("\\");
		std::string fileName = path.substr(desirePos + 1, path.length());
		int uscore = fileName.find("_");
		int dash = fileName.find("-");

		std::stringstream LB(fileName.substr(0, uscore));
		std::stringstream UB(fileName.substr(uscore + 1, dash));
		std::stringstream SB(fileName.substr(dash + 1, fileName.length() - 1));

		T lowerKey, upperKey;
		int lvl;

		LB >> lowerKey;
		UB >> upperKey;
		SB >> lvl;
		  
		if (this->files->count(lvl)){
			return false;
		}
		else{
			 
			this->files->insert(std::make_pair(lvl, fileName));
			this->pairMap.insert(std::make_pair(fileName, std::make_pair(upperKey, lowerKey)));
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

		std::string f;

		f.append(this->baseFolderPath);
		f.append("\\");
		f.append(file);

		boost::filesystem::path p(f);
		boost::filesystem::remove_all(p);
	};

};

#endif // !BLOOMFILTER_H
