#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Predicate.h"
#include "Request.h"
#include <deque> 
#include <iostream>
#include "Merger.h"

#include <functional>
#include <map>

/**********************
* Processor is the actual worker that does all the insert,
* delete and query (view). Each Processor is independent 
* of each other. Thus, they can be partitioned according to 
* the predicate
***********************/
template<class T, class R>
class Processor {
	
private:
	Predicate<T>* predicate;
	std::deque<Request<T, R>> work;
	std::deque<Request<T, R>> queryQueue;

	Merger<T, R> *merger;

public:
	Processor(int level, int ratio, std::string baseFileDir, MergeType mtype, Predicate<T> *p) {
		this->predicate = p;
		this->merger = new Merger<T, R>(level, ratio, baseFileDir, mtype);
	};

	~Processor() {
		delete this->merger; 
		delete this->predicate;
	};

	bool consume(Request<T, R> &request) {
		
		//consume each request
		if (this->predicate->check(request.getKey())) {
			this->work.push_back(request);
			return true;
		}
		else {
			return false;
		}
		
	};

	/*
	* Execute on thread pool
	*/
	void execute(){
		  
		//process all works
		while (!this->work.empty()) {
			
			Request<T, R> req = this->work.front();

			if (req.getType() == INSERT || req.getType() == REMOVE) { 
				this->merger->insert(req);
			}
			else if (req.getType() == QUERY) { 
				this->queryQueue.push_back(req);
			}

			this->work.pop_front();
		} 
		 
		//invoke check query
		this->merger->query(this->queryQueue);

		//invoke merge 
		this->merger->merge();
		  
	};

	/*
	* Get Query Work Queue
	*/
	std::deque<Request<T, R>>& getQueryWork () {
		return this->queryQueue;
	};

	std::deque<Request<T, R>>& getWork(){
		return this->work;
	};

	std::map<T, Request<T, R>, std::function<bool(const T&, const T&)>>* &getMap(){
		return this->merger->getMap();
	}

};

#endif // !PROCESSOR_H
