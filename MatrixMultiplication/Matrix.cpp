#include <iostream>
#include <fstream>
#include "Matrix.h"

//	M and N initialized by member initializer list
Matrix::Matrix(size_t _m, size_t _n) : m(_m), n(_n)
{
	// allocate M x N matrix
	content = new float*[m];
	for (size_t i = 0; i < m; i++)
	{
		content[i] = new float[n];
	}
}

Matrix::Matrix(std::string fileName)
{
	// open input file
	std::ifstream file;
	file.open(fileName, std::ifstream::in);

	// read te matrix size
	file >> m >> n;

	// allocate M x N matrix using previous constructor
	Matrix(m, n);

	// fill the matrix by file conent
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			file >> content[i][j];
		}
	}

	// close the file
	file.close();
}

Matrix::~Matrix()
{
	// free all the memory
	for (size_t i = 0; i < m; i++)
	{
		delete[] content[i];
	}

	delete[] content;
}

void Matrix::writeToFile(std::string fileName)
{
	// open the file
	std::ofstream file;
	file.open(fileName, std::ofstream::out);

	// save the dimentions
	file << m << " " << n << std::endl;

	// save the content
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			file << content[i][j] << " ";
		}
		file << std::endl;
	}

	// close the file
	file.close();
}
