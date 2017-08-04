#include <iostream>
#include "Type.h"
#include "CudaParallel.h"
int main()
{
	using namespace std;


	CalculateSinValue(ARRAY_SIZE);
	
	return 0;
}