#pragma once

#include <string>

class Matrix
{
private:
	size_t m, n;
	float** content;

public:
	// allocate memory for empity matrix M x N
	Matrix(size_t _m, size_t _n);

	// allocate memory for the matrix and read the content from file
	Matrix(std::string fileName);
	
	// free the memory allocated for the matrix
	~Matrix();

	// getters
	float** getContent() { return content; }
	size_t getM() const { return m; }
	size_t getN() const { return n; }
	float getAt(size_t i, size_t j) const { return content[i][j]; }

	// write the matrix to file
	void writeToFile(std::string fileName);
	void setAt(size_t i, size_t j, float value);
};
