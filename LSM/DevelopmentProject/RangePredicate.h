#ifndef RANGEPREDICATE_H
#define RANGEPREDICATE_H

#include "Predicate.h"

template<class T>
class RangePredicate : public Predicate<T>{

public:
	RangePredicate(T lower, T upper) {
		this->lowerBound = lower;
		this->upperBound = upper;
	};

	bool check(T key) {
		
		return (this->lowerBound <= key) && (this->upperBound > key);

	};

private:
	T lowerBound;
	T upperBound;

};

#endif // !RANGEPREDICATE_H
