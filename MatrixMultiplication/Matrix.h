#pragma once

#include <string>

class Matrix
{
private:
	int m, n;
	float* content;

public:
	// allocate memory for empity matrix M x N
	Matrix(int _m, int _n);

	// allocate memory for the matrix and read the content from file
	Matrix(std::string fileName);
	
	// free the memory allocated for the matrix
	~Matrix();

	// getters
	float* getContent() { return content; }
	int getM() const { return m; }
	int getN() const { return n; }
	float getAt(int i, int j) const { return content[i * m + j]; }

	// write the matrix to file
	void writeToFile(std::string fileName);
	// set matrix field value
	void setAt(int i, int j, float value);
};
