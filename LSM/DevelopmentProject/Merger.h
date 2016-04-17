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
#include "Utils.h"

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

	std::deque<Request<T, R>> insertRequests;

	MergeType type;

	CudaDevice<T, R> *device;

	BloomFilter<T> *filter;

	FileAccessor<T, R> *fileAccess;

	void requestMerge(Request<T, R>* &arrA, Request<T, R>* &arrB, int size, Request<T, R>* &mergedArray){
	 
		if (this->type == MergeType::DEVICE && this->device->isCudaAvailable()){
			this->invokeGPUmerge(arrA, size, arrB, size, mergedArray); 
		}
		else{
			this->mergeCPU(arrA, arrB, size, mergedArray);
		}
	};
	 
	int computeLevel(int len){
		return len / (this->level * this->ratio);
	};

public:
	 
	/*
	* Constructor
	* level - The number of level to be stored on RAM
	* ratio - This is the size ratio between each level
	* fileDir - The location to save the file
	*/
	Merger(int level, int ratio, std::string filedir, MergeType type) : level(level), ratio(ratio), fileDir(filedir), type(type) {
	  
		this->filter = new BloomFilter<T>(fileDir, this->level);

		this->device = new CudaDevice<T, R>();

		auto cmp = [](const T& a, const T& b) { return a < b; };
		this->C0 = new std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>>(cmp);

		this->fileAccess = new FileAccessor<T, R>(filedir);
	};

	/*
	* Overload of constructor
	*/
	Merger(std::string fdir) : Merger(1024, 1, fdir, MergeType::ONBOARD)  {};

	/*
	* Destructor 
	*/
	~Merger(){
		delete this->filter;
		delete this->C0;
		//delete this->device;
		delete this->fileAccess;
	};

	/*
	* Process Read Query 
	*/
	void processReadQuery(std::deque<Request<T, R>> &requests, std::deque<Request<T, R>> &completed, std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> &tree){
		
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


		//copy completed requests to pinging requests
		int completedSize = completed.size();

		while (completedSize > 0){
		
			requests.push_back(completed.front());
			completed.pop_front();

			completedSize--;
		}
	};

	
	/*
	* Recursive Merging all the tree
	*/
	void recursiveMerge(int currLevel, Request<T, R>* &Ltree, std::map<int, std::string, std::function<bool(const int&, const int&)>> *bloomFilter, int mapSize){
	   
		Request<T, R> *ptr = nullptr;

		int length = mapSize;
		currLevel = computeLevel(length);

		if (bloomFilter->count(currLevel) == 1){
			
			Request<T, R>* B = nullptr;
			int Bsize = 0;
			auto cmp = [](const T& a, const T& b) { return a < b; };
			std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> cTreeMap(cmp);

			std::string file = bloomFilter->at(currLevel);

			//read from disk 
			this->fileAccess->readFile(file, cTreeMap);
			Utils<T, R>::createArray(cTreeMap, B, Bsize);
			  
			this->filter->remove(currLevel); // remove the old file

			//invoke merge 
			this->requestMerge(Ltree, B, mapSize, ptr);

			length = mapSize + Bsize;

			//increment level
			currLevel = currLevel + 1;
			 
			//free resources
			delete B;
			 
		}
		else{  
			//save to file
			T bot = Ltree[0].getKey();
			T top = Ltree[length - 1].getKey();
			 
			std::string fn = Utils<T, R>::createFileName(bot, top, currLevel);

			fileAccess->writeFile(fn, Ltree, length);

			this->filter->update(fn);

			return;
		}

		//determine if it needs to make another merge
		if (bloomFilter->count(currLevel) == 1){
			
			this->recursiveMerge(currLevel, ptr, bloomFilter, length);

		}
		else{
			//save to file to disk 
			T bot = ptr[0].getKey();
			T top = ptr[length - 1].getKey();  

			std::string fn = Utils<T, R>::createFileName(bot, top, computeLevel(length));
			  
			fileAccess->writeFile(fn, ptr, length);
			
			this->filter->update(fn);
		}

		//free the resources
		delete ptr;
	};

	/*
	* Check C0 tree and see if it should be merged or save to disk
	*/
	void merge() {

		//check if the file meets the file limit (1st level) 
		if (this->C0->size() < (this->level * this->ratio)){
			return;
		}

		int currLevel = 1;

		//merge 
		std::map<int, std::string, std::function<bool(const int&, const int&)>> *diskMap = this->filter->getLevelMap();

		auto cmp = [](const T& a, const T& b) { return a < b; };
		std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>> cTreeMap(cmp);

		Request<T, R>* A;
		A = (Request<T, R>*)malloc(sizeof(Request<T, R>) * this->C0->size());
		int Asize = 0;

		Utils<T, R>::createArray(*this->C0, A, Asize); 

		this->recursiveMerge(currLevel, A, diskMap, Asize);

		//free resources
		delete A;
		delete diskMap;
		this->C0->clear();
	};


	/*
	* CPU merge version
	*/
	void mergeCPU(Request<T, R>* &arrA, Request<T, R>* &arrB, int size, Request<T, R>* &mergedArray){
		
		int msize = size * 2;

		//create dynamic array
		mergedArray = (Request<T, R>*)malloc(sizeof(Request<T, R>) * msize);

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
	void mergeGPU(Request<T, R>* arrA, Request<T, R>* &arrB, int size, Request<T, R>* mergedArray, int arrSize){
	  
		//create index for A
		int *idxB = new int[size]; 

		for (int i = 0; i < size; i++){ 
			idxB[i] = i;
		}

		//invoke merge kernel to find B indices in respective to A
		this->device->mergeKernel(arrA, (arrSize - size), arrB, idxB, size);
  
		int mIdx = 0;

		int aix = 0;
		int bix = 0;

		//place merged item into an array
		while (mIdx < arrSize){
		
			if (idxB[bix] == mIdx){
				mergedArray[mIdx] = arrB[bix];
				bix++;
			}
			else{
				mergedArray[mIdx] = arrA[aix];
				aix++;
			}

			mIdx++;
		} 
		 
		//free resource and return sorted vector
		delete idxB; 
	};

	void invokeGPUmerge(Request<T, R>* arrA, int arrAsize, Request<T, R>* arrB, int arrBsize, Request<T, R>* &mergedArray){
	
		mergedArray = (Request<int, int>*)malloc(sizeof(Request<int, int>) * (arrAsize + arrBsize));

		//append A to the end of B
		if ((arrA[arrAsize - 1].getKey() < arrB[0].getKey())){
		 
			for (int i = 0; i < arrAsize; i++){
				mergedArray[i] = arrA[i];
			}

			for (int i = arrAsize; i < arrAsize + arrBsize; i++){
				mergedArray[i] = arrB[i - arrAsize];
			}

			return;
		}


		//append B to the end of A
		if (arrB[arrBsize - 1].getKey() < arrA[0].getKey()){

			for (int i = 0; i < arrBsize; i++){
				mergedArray[i] = arrB[i];
			}

			for (int i = arrBsize; i < arrAsize + arrBsize; i++){
				mergedArray[i] = arrA[i - arrBsize];
			}

			return;
		}

		//mix A and B 
		if (arrAsize < arrBsize){
			mergeGPU(arrB, arrA, arrAsize, mergedArray, (arrAsize + arrBsize));
		}
		else{
			mergeGPU(arrA, arrB, arrBsize, mergedArray, (arrAsize + arrBsize));
		}

	};

};

#endif // !MERGER_H
