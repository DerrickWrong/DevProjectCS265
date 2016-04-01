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
#include "BloomFilter.h"

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
		
		//read local cache


		//read every single file on disk 

	};

	/*
	* Check C0 tree and see if it should be merged or save to disk
	*/
	void merge() {

		//check if the file meets the file limit (level) 
		if (this->C0->size() < this->level){
			return;
		}

		Request<T, R> *tempTree;
		tempTree = (Request<T, R>*)malloc(sizeof(Request<T, R>) * this->level);

		int counter = 0;

		//Perform merge for equal size files	
		for (std::map<T, Request<T, R>>::iterator it = this->C0->begin(); it != this->C0->end(); ++it){
			
			tempTree[counter] = it->second;
			counter = counter + 1;

			//remove 
			this->C0->erase(it->first);

			//break the loop
			if (counter == this->level){
				break;
			}
		}
		
		int level = 0;



		//free resources
		delete tempTree;
		
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
		int *idxA = new int[size];
		int *idxB = new int[size];
		
		for (int i = 0; i < size; i++){
			idxA[i] = i;
			idxB[i] = i;
		}

		//invoke merge kernel to find A respective to B
		this->device->mergeKernel(arrA, arrB, idxB, size);
		this->device->mergeKernel(arrB, arrA, idxA, size);


		//place merged item into an array
		for (int i = 0; i < size; i++){
			mergedArray[idxA[i]] = arrA[i];
			mergedArray[idxB[i]] = arrB[i];
		}

		//free resource and return sorted vector
		delete idxA;
		delete idxB;
	};


};

#endif // !MERGER_H
