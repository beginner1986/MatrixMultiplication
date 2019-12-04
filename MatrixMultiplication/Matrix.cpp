#include <iostream>
#include <fstream>
#include "Matrix.h"

//	M and N initialized by member initializer list
Matrix::Matrix(int _m, int _n) : m(_m), n(_n)
{
	// allocate M x N matrix
	content = new float[m * n];
}

Matrix::Matrix(std::string fileName)
{
	// open input file
	std::ifstream file;
	file.open(fileName, std::ifstream::in);

	try {
		file.exceptions(file.failbit);
	}
	catch (const std::ios_base::failure & e)
	{
		std::cout << "Nie mo¿na otworzyæ pliku" << std::endl;
		exit(0);
	}

	// read the matrix size
	file >> m >> n;

	// allocate M x N matrix
	content = content = new float[m * n];

	// fill the matrix with file conent
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			file >> content[i * m + j];
		}
	}

	// close the file
	file.close();
}

Matrix::~Matrix()
{
	// free all the memory
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
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			file << content[i * m + j] << " ";
		}
		file << std::endl;
	}

	// close the file
	file.close();
}

void Matrix::setAt(int i, int j, float value)
{
	content[i * m + j] = value;
}
