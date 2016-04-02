#ifndef MERGER_H
#define MERGER_H

#include <string>
#include <map>
#include <deque>
#include <vector> 
#include <functional>

#include "CudaDevice.h"
#include "BloomFilter.h"
#include "Request.h" 
#include "FileAccessor.h"

enum MergeType {
	ONBOARD,
	DEVICE
};
 
template<class T, class R>
class Merger {

private:
	int level, ratio;
	std::string fileDir;
	
	std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> *C0;

	MergeType type;

	CudaDevice<T, R> *device;

	BloomFilter<T> *filter;

	FileAccessor<T, R> *fileAccess;

	void requestMerge(Request<T, R> *arrA, Request<T, R> *arrB, int size, Request<T, R> *mergedArray){
	 
		if (this->type == MergeType::DEVICE && this->device->isCudaAvailable()){
			this->mergeGPU(arrA, arrB, size, mergedArray);
		}
		else{
			this->mergeCPU(arrA, arrB, size, mergedArray);
		}
	};


public:
	 
	/*
	* Constructor
	* level - The number of level to be stored on RAM
	* ratio - This is the size ratio between each level
	* fileDir - The location to save the file
	*/
	Merger(int level, int ratio, std::string filedir, MergeType type) : level(level), ratio(ratio), fileDir(filedir), type(type) {
	  
		this->filter = new BloomFilter<T>(fileDir);

		this->device = new CudaDevice<T, R>();

		auto cmp = [](const T& a, const T& b) { return a < b; };
		this->C0 = new std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>>(cmp);

		this->fileAccess = new FileAccessor<T, R>(filedir);
	};

	/*
	* Overload of constructor
	*/
	Merger(std::string fdir) : Merger(1024, 2, fdir, MergeType::ONBOARD)  {};

	/*
	* Destructor 
	*/
	~Merger(){
		delete this->filter;
		delete this->C0;
		delete this->device;
		delete this->fileAccess;
	};

	/*
	* Process Read Query 
	*/
	static void processReadQuery(std::deque<Request<T, R>> &requests, std::deque<Request<T, R>> &completed, std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> &tree){
		
		int size = requests.size(); 

		while (size > 0){
			
			Request<T, R> req = requests.front();
			requests.pop_front();

			if (tree.count(req.getKey()) == 1){
				Request<T, R> v = tree.at(req.getKey());
				req.setValue(v.getValue());
				completed.push_back(req);
			}
			else{
				requests.push_back(req);
			}

			size--;
		} 
	};

	static Request<T, R> *convertMap(std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> &treeMap){
		
		Request<T, R> *arr = (Request<T, R>*)malloc(sizeof(Request<T, R>) * treeMap.size());
		
		std::map<T, Request<T, R>>::iterator it;

		int counter = 0;

		for (it = treeMap.begin(); it != treeMap.end(); it++){
		
			arr[counter] = it->second;
			counter = counter + 1;
		}
		  
		return arr;
	};


	/*
	* Process all insert, update and delete requests
	*/
	void insert(Request<T, R> &request) {
	
		this->C0->insert(std::make_pair(request.getKey(), request));
	};

	/*
	* Process all the requests for viewing
	*/
	void query(std::deque<Request<T, R>> &requests) {
		
		//check C0 
		std::deque<Request<T, R>> completed;
		
		this->processReadQuery(requests, completed, *this->C0);

		//read every single file on disk 
		std::map<int, std::string, std::function<bool(const int&, const int&)>> *kMap = this->filter->getLevelMap();

		std::map<int, std::string>::iterator it;

		auto cmp = [](const T& a, const T& b) { return a < b; };
		std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> ctree(cmp);

		for (it = kMap->begin(); it != kMap->end(); it++){
			std::string fn = it->second;
			  
			if (requests.empty()){
				break;
			}
			else{
				this->fileAccess->readFile(fn, ctree);
				this->processReadQuery(requests, completed, ctree);
			}
		}

		requests = completed;
	};

	
	/*
	* Recursive Merging all the tree
	*/
	void recursiveMerge(int currLevel, Request<T, R>* Ltree, std::map<int, std::string, std::function<bool(const int&, const int&)>> *bloomFilter){
	 
		Request<T, R> *ptr;

		if (bloomFilter->count(currLevel) == 1){
			
			Request<T, R>* B;

			auto cmp = [](const T& a, const T& b) { return a < b; };
			std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> cTreeMap(cmp);

			std::string file = bloomFilter->at(currLevel);

			//read from disk 
			this->fileAccess->readFile(file, cTreeMap);
			B = Merger<T, R>::convertMap(cTreeMap);

			//create dynamic array
			ptr = (Request<T, R>*)malloc(sizeof(Request<T, R>) * (currLevel * this->level));
			
			//invoke merge
			this->requestMerge(Ltree, B, (currLevel * this->level), ptr);

			//increment level
			currLevel = currLevel + 1;
			 
			//free resources
			delete B;
			 
		}
		else{ 
			return;
		}

		//determine if it needs to make another merge
		if (bloomFilter->count(currLevel) == 1){
			
			this->recursiveMerge(currLevel, ptr, bloomFilter);

		}
		else{
			//save to file to disk
			int length = (currLevel - 1) * this->level;
			T bot = ptr[0].getKey();
			T top = ptr[length - 1].getKey();

			std::string filename;

			filename.append(reinterpret_cast<char*>(&bot));
			filename.append("_");
			filename.append(reinterpret_cast<char*>(&top));
			filename.append("-");
			filename.append(reinterpret_cast<char*>(&length));

			fileAccess->writeFile(filename, ptr, length);
		}

		//free the resources
		delete ptr;
	};

	/*
	* Check C0 tree and see if it should be merged or save to disk
	*/
	void merge() {

		//check if the file meets the file limit (1st level) 
		if (this->C0->size() < this->level){
			return;
		}

		int currLevel = 1;

		//merge 
		std::map<int, std::string, std::function<bool(const int&, const int&)>> *diskMap = this->filter->getLevelMap();

		auto cmp = [](const T& a, const T& b) { return a < b; };
		std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> cTreeMap(cmp);

		Request<T, R>* A;
		A = Merger<T, R>::convertMap(*this->C0);

		this->recursiveMerge(currLevel, A, diskMap);

		//free resources
		delete A;
		delete diskMap;
	};


	/*
	* CPU merge version
	*/
	void mergeCPU(Request<T, R> *arrA, Request<T, R> *arrB, int size, Request<T, R> *mergedArray){
		
		int msize = size * 2;

		std::vector<Request<T, R>> vec;

		//store to container
		for (int i = 0; i < size; i++){
			vec.push_back(arrA[i]);
			vec.push_back(arrB[i]);
		}
		 
		//apply sort  
		std::sort(vec.begin(), vec.end(), [](Request<T, R> a, Request<T, R> b){
			
			return a.getKey() < b.getKey();

		});

		int counter = 0;
		
		//write data to array
		while (counter < msize){
			mergedArray[counter] = vec.front();
			vec.pop_back();
			counter++;
		}
		 
	};

	/*
	* GPU merge version
	*/
	void mergeGPU(Request<T, R> *arrA, Request<T, R> *arrB, int size, Request<T, R> *mergedArray){
	
		//create index for A
		int *idxB = new int[size];

		for (int i = 0; i < size; i++){
			idxB[i] = i;
		}

		//invoke merge kernel to find B indices in respective to A
		this->device->mergeKernel(arrA, arrB, idxB, size);

		//place merged item into an array
		for (int i = 0; i < size; i++){
			mergedArray[idxB[i]] = arrB[i];   
		}

		//free resource and return sorted vector
		delete idxB;
	};

};

#endif // !MERGER_H
