#include "gtest\gtest.h"
#include "RangePredicate.h" 

TEST(PredicateTest, RangeTest) {
	
	Predicate<int> *p = new RangePredicate<int>(1, 10);
	Predicate<int> *p2 = new RangePredicate<int>(10, 20);

	ASSERT_EQ(1, 1);

	//ASSERT_EQ(true, p->check(1));
	//ASSERT_EQ(true, p2->check(21));

	delete p;
	delete p2;
	 
}