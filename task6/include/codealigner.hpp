#ifndef CODEALIGNER_HPP
#define CODEALIGNER_HPP

#include <string>
#include <algorithm>
#include "stack.hpp"

class CodeAligner {
public:
	CodeAligner(const std::string& old_code, const std::string& new_code,
		int gapValue, int matchValue, int mismatchValue,
		int structMatchValue, int structMismatchValue);

	~CodeAligner();

	void set(const std::string& old_code, const std::string& new_code,
		int gapValue, int matchValue, int mismatchValue,
		int structMatchValue, int structMismatchValue);
	
	std::string align();

private:
	
	enum Edit {
		MATCH = '.',
		MISMATCH = 'M',
		DELETE = '-',
		INSERT = '+'
	};

	enum Direction {
		LEFT,
		UP,
		DIAGONAL
	};

	int** score_matrix = nullptr;
	Direction** dir_matrix = nullptr;

	int gapScore;
	int matchScore;
	int mismatchScore;
	int structMatchScore;
	int structMismatchScore;

	size_t cols = 0;
	size_t rows = 0;

	std::string old_code; 
	std::string new_code;

	int evaluate(const char, const char);

	std::string traceBack();


	template<typename T>
	T** getMatrix(const size_t rows, const size_t cols);

	template<typename T>
	void deleteMatrix(T** matrix, const size_t rows);

};


template<typename T>
void CodeAligner::deleteMatrix(T** matrix, const size_t rows) {
	if (!matrix) return;
	for (size_t i = 0; i < rows; ++i)
		delete[] matrix[i];
	delete[] matrix;
}

template<typename T>
T** CodeAligner::getMatrix(const size_t rows, const size_t cols) {
	if (!rows || !cols) return nullptr;
	T** matrix = new T * [rows];
	for (size_t i = 0; i < rows; ++i)
		matrix[i] = new T[cols];
	return matrix;
}

#endif
