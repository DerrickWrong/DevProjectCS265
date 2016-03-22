#include "gtest\gtest.h"
#include "ThreadPool.h"
#include "ThreadPoolSingleton.h"
  
TEST(ThreadpoolTest, testpool) {

	ThreadPool &pool_1 = ThreadPoolSingleton::getInstance();
	ThreadPool &pool_2 = ThreadPoolSingleton::getInstance();

	ASSERT_EQ(&pool_1, &pool_2);

}