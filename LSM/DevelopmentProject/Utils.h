#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <functional>
#include <sstream>
#include <string>

#include "Request.h"

template<typename T, typename R>
class Utils{

public:
	static void createArray(std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> &treeMap, Request<T, R>* &arr, int &size){
	
		size = treeMap.size();
		arr = (Request<T, R>*)malloc(sizeof(Request<T, R>) * size);
		
		std::map<T, Request<T, R>>::iterator it;

		int counter = 0;

		for (it = treeMap.begin(); it != treeMap.end(); it++){

			arr[counter] = it->second;
			counter = counter + 1;
		} 
	};

	static std::string &createFileName(T bottom, T top, int sizeLevel){
	
		std::string fileName;

		std::ostringstream bottom_os, top_os, lenght_os;
		bottom_os << bottom;
		fileName.append(bottom_os.str());
		fileName.append("_");
		top_os << top;
		fileName.append(top_os.str());
		fileName.append("-");
		lenght_os << sizeLevel;
		fileName.append(lenght_os.str());

		return fileName;
	};

};

#endif