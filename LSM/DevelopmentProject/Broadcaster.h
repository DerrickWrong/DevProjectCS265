#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <deque> 
#include <vector>
#include "Request.h"
#include "Processor.h" 
#include "ThreadPoolSingleton.h"

template<class T, class R>
class Broadcaster {

private:
	std::deque<Request<T, R>> requestQueue;
	Timer* timer;
	uint64_t tid;
	std::vector<Processor<T, R>> workers; 
	 
public:
	Broadcaster() {
		this->timer = new Timer();
		this->tid = this->timer->create(0, 1000, std::bind(&Broadcaster::execute, this));
	};
	
	/*
	* Subscribe a subscriber to worker queue
	*/
	void subscribe(Processor<T, R> sub) {
		this->workers.at(sub);
	};

	/*
	* Thread run from the timer thread
	*/
	void execute() {
		   
		//clean out the queue
		while (this->requestQueue.size() > 0) {
			
			// retrieve work
			Request<T, R> request = this->requestQueue.front();

			//distribute work to the right subscriber 
			for (int w = 0; w < this->workers.size(); w++) {
				
				Processor<T, R> sub = this->workers->at(w);
				
				//consumed work
				if (sub->consume(request)) {
					break;
				}
			}

			//remove work
			this->requestQueue.pop_front();
		}

		//invoke subscriber(s) to work
		for (int w = 0; w < this->workers.size(); w++) {

			Processor<T, R> node = this->workers->at(w);

			ThreadPoolSingleton::getInstance().enqueue(std::bind(&Processor<T,R>::execute, &node));
		} 
		
	};

	~Broadcaster() {
		this->timer->destroy(this->tid);
		delete this->timer;
	}
	
	/*
	* Push in request
	* @input Request
	*/
	void accept(Request<T, R> request) {
		this->requestQueue.push_back(request);
	};
	  
};

#endif // !BROADCASTER_H
