#ifndef THREADPOOLSINGLETON_H
#define THREADPOOLSINGLETON_H

#include "ThreadPool.h"
#include <thread>

class ThreadPoolSingleton {
 
public:
	static ThreadPool &getInstance() {
		
		int numCores = std::thread::hardware_concurrency();

		static ThreadPool p(numCores);

		return p; 
	};

};

#endif // !THREADPOOLSINGLETON_H
