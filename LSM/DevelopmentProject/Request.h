#ifndef REQUEST_H
#define REQUEST_H
 
#include "cuda_runtime.h"  

enum RequestType {
	INSERT,
	REMOVE,
	QUERY
};

template<class T, class P> class Request {
	 
public: 
	/*
	* Reserved for insert/update
	*/
	__host__ __device__ Request(long timestamp, T key, P value) {
		this->timestamp = timestamp;
		this->type = type;
		this->key = key;
		this->type = INSERT;
	}

	/**
	* Reserved for processing Delete and Query
	*/
	__host__ __device__ Request(long timestamp, T key, RequestType type) {
		
		this->timestamp = timestamp;
		this->key = key;
		this->type = type;
	}

	__host__ __device__ void setValue(P value) {
		this->value;
	}

	__host__ __device__ long getTimestamp() {
		return this->timestamp;
	}

	__host__ __device__ T getKey() {
		return this->key;
	}

	__host__ __device__ P getValue() {
		return this->value;
	}

	__host__ __device__ RequestType getType() {
		return this->type;
	}

	int getSize(){
		return sizeof(T) + sizeof(P);
	}

private:
	long timestamp;
	T key;
	P value;
	RequestType type;

};

#endif // !REQUEST_H
