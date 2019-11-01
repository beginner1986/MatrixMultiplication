#include <iostream>
#include "Matrix.h"

int main()
{
	Matrix *A = new Matrix(2, 3);

	std::cout << A->getM() << " " << A->getN() << std::endl;

	delete A;

	std::cin.get();
	return 0;
}
