
#include "codealigner.hpp"

CodeAligner::CodeAligner(const std::string& old_code, const std::string& new_code,
	int gapValue = -2, int matchValue = 1, int mismatchValue = -1,
	int structMatchValue = 3, int structMismatchValue = -3) {
	set(old_code, new_code, gapValue, matchValue, mismatchValue,
		structMatchValue, structMismatchValue);
}
CodeAligner::~CodeAligner() {
	deleteMatrix(this->score_matrix, rows);
	deleteMatrix(this->dir_matrix, rows);
	this->score_matrix = nullptr;
	this->dir_matrix = nullptr;
}

void CodeAligner::set(const std::string& old_code, const std::string& new_code,
	int gapValue, int matchValue, int mismatchValue,
	int structMatchValue, int structMismatchValue) {
	deleteMatrix(this->score_matrix, rows);
	deleteMatrix(this->dir_matrix, rows);
	this->cols = new_code.length() + 1;
	this->rows = old_code.length() + 1;
	this->new_code = new_code;
	this->old_code = old_code;
	this->gapScore = gapValue;
	this->matchScore = matchValue;
	this->mismatchScore = mismatchValue;
	this->structMatchScore = structMatchValue;
	this->structMismatchScore = structMismatchValue;
	this->score_matrix = getMatrix<int>(rows, cols);
	this->dir_matrix = getMatrix<Direction>(rows, cols);

	for (size_t i = 0; i < rows; ++i) {
		score_matrix[i][0] = gapScore * i;
		dir_matrix[i][0] = Direction::UP;
	}

	for (size_t i = 0; i < cols; ++i) {
		score_matrix[0][i] = gapScore * i;
		dir_matrix[0][i] = Direction::LEFT;
	}

	dir_matrix[0][0] = Direction::DIAGONAL;
}


std::string CodeAligner::align() {
	for (size_t i = 1; i < rows; ++i) {
		for (size_t j = 1; j < cols; ++j) {
			int diagValue = score_matrix[i - 1][j - 1] + evaluate(old_code[i - 1], new_code[j - 1]);
			int upValue = score_matrix[i - 1][j] + gapScore;
			int leftValue = score_matrix[i][j - 1] + gapScore;

			int maxValue = diagValue;
			Direction maxDir = Direction::DIAGONAL;
			if (upValue > maxValue) {
				maxValue = upValue;
				maxDir = Direction::UP;
			}

			if (leftValue > maxValue) {
				maxValue = leftValue;
				maxDir = Direction::LEFT;
			}

			score_matrix[i][j] = maxValue;
			dir_matrix[i][j] = maxDir;

		}
	}
	return traceBack();
}

std::string CodeAligner::traceBack() {
	Stack<Edit> edits;
	for (size_t i = rows - 1, j = cols - 1; i > 0 || j > 0;) {
		Direction dir = dir_matrix[i][j];
		switch (dir) {
		case Direction::DIAGONAL:
			if (old_code[i - 1] == new_code[j - 1])
				edits.push(Edit::MATCH);
			else
				edits.push(Edit::MISMATCH);
			--i;
			--j;
			break;
		case Direction::UP:
			edits.push(Edit::DELETE);
			--i;
			break;
		case Direction::LEFT:
			edits.push(Edit::INSERT);
			--j;
			break;
		default:
			break;
		}
	}
	std::string edit_string = "";
	while (!edits.empty()) {
		edit_string += edits.top();
		edits.pop();
	}
	return edit_string;
}

int CodeAligner::evaluate(const char a, const char b) {
	static const std::string whitespace = " \t\n\r ";
	static const std::string structural = "{}();,<>[]#";

	bool a_ws = (whitespace.find(a) != std::string::npos);
	bool b_ws = (whitespace.find(b) != std::string::npos);

	bool a_struct = (structural.find(a) != std::string::npos);
	bool b_struct = (structural.find(b) != std::string::npos);

	if (a_ws && b_ws) return 0;

	if (a == b) 
		return (a_struct) ? structMatchScore : matchScore;
	
	return (a_struct || b_struct) ? structMismatchScore : mismatchScore;
}