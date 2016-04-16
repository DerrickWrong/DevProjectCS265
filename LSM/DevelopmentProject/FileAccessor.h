#ifndef FILEACCESSOR_H
#define FILEACCESSOR_H

#include "Request.h"
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <functional> 

template<class T, class P>
class FileAccessor{

private:
	std::string baseDir;  

public:

	FileAccessor(std::string baseFolder) {
		this->baseDir = baseFolder;
	};

	~FileAccessor(){
	};
	 

	/*
	* Read Data from file on the disk
	*/
	void readFile(std::string filename, std::map<T, Request<T, P>, std::function<bool(const T&, const T&)>> &ctree){

		ctree.clear();

		std::string fileFullDir = baseDir + "\\" + filename;
		 
		T _key;
		P _val;
		long timestamp;

		std::ifstream myFile(fileFullDir, std::ios::in | std::ios::binary);

		if (myFile.is_open()){ 

			while (myFile){
			
				myFile.read(reinterpret_cast<char*>(&_key), sizeof(T));
				myFile.read(reinterpret_cast<char*>(&_val), sizeof(P));

				Request<T, P> request(timestamp, _key, _val);
				ctree.insert(std::make_pair(_key, request));
			}

			myFile.close();
		} 
	}; 

	/*
	* Write Data to file on the disk
	*/
	void writeFile(std::string filename, Request<T, P>* &ctree, int size){
		
		std::string fileFullDir = baseDir + "\\" + filename;

		std::ofstream file;
		file.open(fileFullDir, std::ios::binary);
  
		T key, pkey;
		P value, pval;
		long timestamp;
		
		for (int i = 0; i < size; i++){ 
			
			key = ctree[i].getKey();
			value = ctree[i].getValue();
			timestamp = ctree[i].getTimestamp();
			
			//remove duplicate 
			if (i > 0 && pkey == key){
				continue;
			}

			//this->write(file, timestamp, key, value);
			file.write(reinterpret_cast<char*>(&key), sizeof(T));
			file.write(reinterpret_cast<char*>(&value), sizeof(P));
			
			//assign previous values
			pkey = key;
			pval = value;
		}

		file.close();
		
	};

};

#endif 