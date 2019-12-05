#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <vector>
#include <omp.h>
#include "Matrix.h"

double multiplyI(const Matrix& A, const Matrix& B, Matrix& result, int threadsNumber);
double multiplyJ(const Matrix& A, const Matrix& B, Matrix& result, int threadsNumber);
double multiplyK(const Matrix& A, const Matrix& B, Matrix& result, int threadsNumber);
void printTable(double table[5][6]);

int main()
{
	// polish language support
	setlocale(LC_ALL, "polish");
	std::cout << std::setprecision(5) << std::fixed;

	// read the matrices to multiply
	Matrix* A, * B;
	std::vector<int> sizes{ 100, 500, 1000, 2000 };

	// result arrays rows initialization
	double timesI[5][6];
	double timesJ[5][6];
	double timesK[5][6];

	// top - left field
	timesI[0][0] = 0;
	timesJ[0][0] = 0;
	timesK[0][0] = 0;
	
	// first column (sizes)
	for (size_t i=0; i<sizes.size(); i++)
	{
		timesI[i + 1][0] = sizes[i];
		timesJ[i + 1][0] = sizes[i];
		timesK[i + 1][0] = sizes[i];
	}

	// test cases
	for (int numberOfThreads = 1, column = 1; numberOfThreads <= 16; numberOfThreads *= 2, column++)
	{
		// result arrays columns initialization
		timesI[0][column] = numberOfThreads;
		timesJ[0][column] = numberOfThreads;
		timesK[0][column] = numberOfThreads;

		for (size_t row=0; row<sizes.size(); row++)
		{
			int size = sizes[row];

			std::string matrixAFileName = "a" + std::to_string(size);
			std::string matrixBFileName = "b" + std::to_string(size);
			std::string matrixCFileName = "c" + std::to_string(size);

			omp_set_num_threads(2);
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

			// multiply the matrices, save result to file and count time
			timesI[row + 1][column] = multiplyI(*A, *B, *C, numberOfThreads);
			C->writeToFile(matrixCFileName + "i");
			timesJ[row + 1][column] = multiplyJ(*A, *B, *C, numberOfThreads);
			C->writeToFile(matrixCFileName + "j");
			timesK[row + 1][column] = multiplyK(*A, *B, *C, numberOfThreads);
			C->writeToFile(matrixCFileName + "k");

			// free the memory
			delete A, B, C;
		}
	}

	// print the times
	std::cout << "i - loop:" << std::endl;
	printTable(timesI);
	std::cout << "j - loop:" << std::endl;
	printTable(timesJ);
	std::cout << "k - loop:" << std::endl;
	printTable(timesK);

	// hold the screen
	std::cin.get();
	int temp;
	std::cin >> temp;
	return 0;
}

void printTable(double table[5][6])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			std::cout << (i==0 || j==0 ? std::setprecision(0) : std::setprecision(5)) << std::fixed << table[i][j] << "\t" << (table[i][j] > 10.0f ? "" : "\t");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
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
