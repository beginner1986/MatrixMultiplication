#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <omp.h>
#include "Matrix.h"

double multiplyI(const Matrix& A, const Matrix& B, Matrix &result, int threadsNumber);
double multiplyJ(const Matrix& A, const Matrix& B, Matrix& result, int threadsNumber);
double multiplyK(const Matrix& A, const Matrix& B, Matrix& result, int threadsNumber);

double timesI[5][6];
double timesJ[5][6];
double timesK[5][6];

int main()
{
	// polish language support
	setlocale(LC_ALL, "polish");

	// read the matrices to multiply
	Matrix* A, * B;
	std::vector<int> sizes{ 100, 500, 1000, 2000 };

	// result arrays rows initialization
	for (size_t i=0; i<sizes.size(); i++)
	{
		timesI[i][0] = sizes[i];
		timesJ[i][0] = sizes[i];
		timesK[i][0] = sizes[i];
	}

	// test cases
	for (int numberOfThreads = 1; numberOfThreads <= 16; numberOfThreads *= 2)
	{

		// result arrays columns initialization
		int column = 0;
		timesI[0][column] = numberOfThreads;

		for (size_t row=0; row<sizes.size(); row++)
		{
			int size = sizes[row];

			std::string matrixAFileName = "a" + std::to_string(size);
			std::string matrixBFileName = "b" + std::to_string(size);
			std::string matrixCFileName = "c" + std::to_string(size);

			#pragma omp parallel
			{
				#pragma omp sections
				{
					#pragma omp section
					{
						A = new Matrix(matrixAFileName);
					}
					#pragma omp section
					{
						B = new Matrix(matrixBFileName);
					}
				}
			}

			// prepare the result matrix
			Matrix *C = new Matrix(A->getN(), B->getM());

			// multiply the matrices
			timesI[row][column] = multiplyI(*A, *B, *C, numberOfThreads);
			C->writeToFile(matrixCFileName + "i");
			timesJ[row][column] = multiplyJ(*A, *B, *C, numberOfThreads);
			C->writeToFile(matrixCFileName + "j");
			timesK[row][column] = multiplyK(*A, *B, *C, numberOfThreads);
			C->writeToFile(matrixCFileName + "k");

			// save result to file

			// free the memory
			delete A, B, C;
		}
	}

	// hold the screen
	std::cin.get();
	return 0;
}

double multiplyI(const Matrix& A, const Matrix& B, Matrix &result, int threadsNumber)
{
	// start time masurement
	auto startTime = std::chrono::system_clock::now();

	// check multiplication correctness condition
	if (A.getN() != B.getM())
	{
		std::cout << "Nie mo¿na pomno¿yæ tych macierzy!" << std::endl;
		exit(-1);
	}
	
	// perform the multiplication
	int i, j, k;
	omp_set_num_threads(threadsNumber);
	#pragma omp parallel shared(A, B, result) private(i, j, k)
	{
  		#pragma omp for
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

	// end time and time difference print
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> time = endTime - startTime;
	std::cout << "Pêtla i" << "\t\t" << "Wymiar: " << A.getM() << "\t" << "w¹tków: " << threadsNumber << "\t" << "czas: " << time.count() << "s." << std::endl;

	return time.count();
}

double multiplyJ(const Matrix& A, const Matrix& B, Matrix& result, int threadsNumber)
{
	// start time masurement
	auto startTime = std::chrono::system_clock::now();

	// check multiplication correctness condition
	if (A.getN() != B.getM())
	{
		std::cout << "Nie mo¿na pomno¿yæ tych macierzy!" << std::endl;
		exit(-1);
	}

	// perform the multiplication
	int i, j, k;
	omp_set_num_threads(threadsNumber);
		for (i = 0; i < A.getM(); i++)
		{
		#pragma omp parallel shared(A, B, result, i) private(j, k)
			{
			#pragma omp for
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

	// end time and time difference print
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> time = endTime - startTime;
	std::cout << "Pêtla j" << "\t\t" << "Wymiar: " << A.getM() << "\t" << "w¹tków: " << threadsNumber << "\t" << "czas: " << time.count() << "s." << std::endl;

	return time.count();
}

double multiplyK(const Matrix& A, const Matrix& B, Matrix& result, int threadsNumber)
{
	// start time masurement
	auto startTime = std::chrono::system_clock::now();

	// check multiplication correctness condition
	if (A.getN() != B.getM())
	{
		std::cout << "Nie mo¿na pomno¿yæ tych macierzy!" << std::endl;
		exit(-1);
	}

	// perform the multiplication
	int i, j, k;
	omp_set_num_threads(threadsNumber);
		for (i = 0; i < A.getM(); i++)
		{
			for (j = 0; j < B.getN(); j++)
			{
				float temp = 0;
				#pragma omp parallel shared(A, B, result, i, j, temp) private(k)
				{
					#pragma omp for
					for (k = 0; k < A.getN(); k++)
					{
						temp = temp + A.getAt(i, k) * B.getAt(k, j);
					}
					result.setAt(i, j, temp);
			}
		}
	};

	// end time and time difference print
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> time = endTime - startTime;
	std::cout << "Pêtla k" << "\t\t" << "Wymiar: " << A.getM() << "\t" << "w¹tków: " << threadsNumber << "\t" << "czas: " << time.count() << "s." << std::endl;

	return time.count();
}
