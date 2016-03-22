#ifndef MERGER_H
#define MERGER_H

#include <string>
#include <map>
#include <deque>
#include "Request.h"
#include <stdio.h>
#include <Windows.h>

enum MergeType {
	ONBOARD,
	DEVICE
};

template<class T, class R>
class Merger {

private:
	int level, ratio;
	std::string fileDir;
	
	std::map<T, Request<T, R>> C0;

	MergeType type;

public:
	 
	/*
	* Constructor
	* level - The number of level to be stored on RAM
	* ratio - This is the size ratio between each level
	* fileDir - The location to save the file
	*/
	Merger(int level, int ratio, std::string filedir, MergeType type) : level(level), ratio(ratio), fileDir(filedir), type(type) {
	  
		//create directory if not exist else read all files from directory
		std::wstring stemp = std::wstring(filedir.begin(), filedir.end()); 

		if (CreateDirectory(stemp.c_str(), NULL))
		{
			// Directory created
		} 
	};

	Merger(std::string fdir) : Merger(1000, 2, fdir, MergeType::ONBOARD)  {};

	/*
	* Process all insert, update and delete requests
	*/
	void modify(Request<T, R> &request) {
		this->C0[request.getKey()] = request;
	};

	/*
	* Process all the requests for viewing
	*/
	void query(std::deque<Request<T, R>> &requests) {
		
		//read local cache

		
		//read every single file on disk 


	};

	/*
	* Check C0 tree and see if it should be merged or save to disk
	*/
	void merge() {
		
		//check and see if the cache is full

	};

};

#endif // !MERGER_H
