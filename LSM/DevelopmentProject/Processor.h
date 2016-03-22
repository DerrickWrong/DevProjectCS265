#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Predicate.h"
#include "Request.h"
#include <deque> 
#include <iostream>


/**********************
* Processor is the actual worker that does all the insert,
* delete and query (view). Each Processor is independent 
* of each other. Thus, they can be partitioned according to 
* the predicate
***********************/
template<class T, class R>
class Processor {
	 
public:
	Processor(Predicate<T> *p) {
		this->predicate = p;
	};

	~Processor() {
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
				
			}
			else if (req.getType() == QUERY) { 
				this->queryQueue.push_back(req);
			}

			this->work.pop_front();
		} 

		//invoke check query

	};

	/*
	* Get Query Work Queue
	*/
	std::deque<Request<T, R>> getQueryWork () {
		return this->queryQueue;
	};

private:
	Predicate<T> *predicate;
	std::deque<Request<T, R>> work;
	std::deque<Request<T, R>> queryQueue;
};

#endif // !PROCESSOR_H
