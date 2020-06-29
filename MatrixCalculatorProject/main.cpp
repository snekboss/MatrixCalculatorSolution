#include "MatrixCalculator.h"
#include <iostream>

/**
* The entry point of the Matrix Calculator. It simple calls the MatrixCalculator::run() method inside a C++ try-catch block
* @see MatrixCalculator::run()
*/
int main()
{
	try
	{
		MatrixCalculator().run();
	}
	catch (const std::exception&)
	{
		std::cout << std::endl << std::endl << "Matrix Calculator encountered an exception. Exiting..." << std::endl << std::endl;
	}
	return 0;
}