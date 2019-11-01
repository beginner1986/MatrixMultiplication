#include <iostream>
#include <string>
#include <chrono>
#include <omp.h>
#include "Matrix.h"

Matrix* readFromFile(std::string matrixName);
void saveToFile(Matrix* matrix);
Matrix* multiply(Matrix* A, Matrix* B);

int main()
{
	// polish language support
	setlocale(LC_ALL, "polish");

	// read the matrices to multiply
	Matrix* A = readFromFile("A");
	Matrix* B = readFromFile("B");

	// prepare the result matrix
	Matrix *C = new Matrix(A->getN(), B->getM());

	// start time masurement
	auto startTime = std::chrono::system_clock::now();

	// multiply the matrices
	C = multiply(A, B);

	// end time and time difference print
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> time = endTime - startTime;
	std::cout << "Obliczenia zajê³y " << time.count() << "s." << std::endl;

	// save result to file
	saveToFile(C);

	// free the memory
	delete A, B, C;

	// hold the screen
	std::cin.get();
	return 0;
}

Matrix* readFromFile(std::string matrixName)
{
	// get file name
	std::string fileName;
	std::cout << "Podaj nazwê pliku z macierz¹ " << matrixName << ": ";
	std::cin >> fileName;

	// create the matrix and return it
	Matrix* result = new Matrix(fileName);
	
	// success info
	std::cout << "Macierz " << matrixName << " zosta³a poprawnie wczytana." << std::endl;

	return result;
}

void saveToFile(Matrix* matrix)
{
	// get file name
	std::string fileName;
	std::cout << "Podaj nazwê pliku wynikowego C = A * B: ";
	std::cin >> fileName;

	// save matrix to file
	matrix->writeToFile(fileName);
}

Matrix* multiply(Matrix* A, Matrix* B)
{
	// check multiplication correctness condition
	if (A->getN() != B->getM())
	{
		std::cout << "Nie mo¿na pomno¿yæ tych macierzy!" << std::endl;
		return nullptr;
	}

	// create result matrix
	Matrix* result = new Matrix(A->getN(), B->getM());

	// perform the multiplication
	int i, j, k;
	#pragma omp parallel shared(A, B, result) private(i, j, k)
	{
		#pragma omp for schedule(static)
		for (i = 0; i < A->getM(); i++)
		{
			for (j = 0; j < B->getN(); j++)
			{
				float temp = 0;
				for (k = 0; k < A->getN(); k++)
				{
					temp += A->getAt(i, k) * B->getAt(k, j);
				}
				result->setAt(i, j, temp);
				std::string iteration = "C(" + std::to_string(i) + "," + std::to_string(j) + ") - w¹tek #" + std::to_string(omp_get_thread_num());
				std::cout << iteration << std::endl;
			}
		}
	}

	return result;
}
