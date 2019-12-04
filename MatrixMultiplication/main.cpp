#include <iostream>
#include <string>
#include <chrono>
#include <omp.h>
#include "Matrix.h"

Matrix* readFromFile(std::string fileName);
void saveToFile(Matrix& matrix);
void multiply(const Matrix& A, const Matrix& B, Matrix &result);

int main()
{
	// polish language support
	setlocale(LC_ALL, "polish");

	// read the matrices to multiply
	Matrix* A, * B;
	std::string matrixAFileName, matrixBFileName;
	std::cout << "Podaj nazwy plików z macierzami [A B]: ";
	std::cin >> matrixAFileName >> matrixBFileName;

	#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				A = readFromFile(matrixAFileName);
			}
			#pragma omp section
			{
				B = readFromFile(matrixBFileName);
			}
		}
	}

	// prepare the result matrix
	Matrix *C = new Matrix(A->getN(), B->getM());

	// start time masurement
	auto startTime = std::chrono::system_clock::now();

	// multiply the matrices
	multiply(*A, *B, *C);

	// end time and time difference print
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> time = endTime - startTime;
	std::cout << "Obliczenia zajê³y " << time.count() << "s." << std::endl;

	// save result to file
	saveToFile(*C);

	// free the memory
	delete A, B, C;

	// hold the screen
	std::cin.get();
	return 0;
}

Matrix* readFromFile(std::string fileName)
{
	// create the matrix and return it
	Matrix* result = new Matrix(fileName);
	
	// success info
	std::cout << "Macierz " << fileName << " zosta³a poprawnie wczytana." << std::endl;

	return result;
}

void saveToFile(Matrix& matrix)
{
	// get file name
	std::string fileName;
	std::cout << "Podaj nazwê pliku wynikowego C = A * B: ";
	std::cin >> fileName;

	// save matrix to file
	matrix.writeToFile(fileName);
}

void multiply(const Matrix& A, const Matrix& B, Matrix &result)
{
	// check multiplication correctness condition
	if (A.getN() != B.getM())
	{
		std::cout << "Nie mo¿na pomno¿yæ tych macierzy!" << std::endl;
		return;
	}
	
	// perform the multiplication
	int i, j, k;
//	#pragma omp parallel shared(A, B, result) private(i, j, k)
	{
//		#pragma omp for schedule(static)
		for (i = 0; i < A.getM(); i++)
		{
			for (j = 0; j < B.getN(); j++)
			{
				float temp = 0;
				for (k = 0; k < A.getN(); k++)
				{
					temp = temp + A.getAt(i, k) * B.getAt(k, j);
				}
				result.setAt(i, j, temp);
			}
		}
	};
}
