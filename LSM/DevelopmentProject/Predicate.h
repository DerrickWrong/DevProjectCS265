#ifndef PREDICATE_H
#define PREDICATE_H

template<class T>
class Predicate {
public:
	virtual bool check(T key) = 0;
};
 
#endif // !PREDICATE_H
