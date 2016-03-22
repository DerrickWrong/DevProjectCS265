#include <iostream>
#include "CudaDevice.h"

int main(){

	CudaDevice<int, int> cuDev;

	std::cout << "bah" << cuDev.isCudaAvailable() << std::endl;

	system("pause");

	return 0;
}