//Nna minkousse Kenneth James
//Belli Andrea
//Premori Samuele

#include <fstream>
#include <string>
#include <iostream>
#include "codealigner.hpp"
#include <algorithm>

std::string fileToString(const char*);
std::string formatter(std::string old_code, std::string new_code, std::string edit_string);
char normalize(const char c);

int main(int argc, char* argv[]) {
	if (argc != 4)
		return 1;

	std::string original = fileToString(argv[1]);
	std::string modified = fileToString(argv[2]);
	std::ofstream out(argv[3]);

	if (!original.length() || !modified.length() || !out.is_open())
		return 1;


	CodeAligner aligner(original, modified, -4, 5, -2, 10, -10);

	std::string edit_string = aligner.align();
	out << formatter(original, modified, edit_string);
	out.close();
	return 0;
}

std::string fileToString(const char* filename) {
	std::ifstream file(filename);
	if (!file.is_open()) return std::string();
	std::string str = std::string(std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>());
	file.close();
	return str;
}

std::string formatter(std::string old_code, std::string new_code, std::string edit_string) {
	std::string res = "";
	for (size_t i = 0, j = 0; i < edit_string.length(); ++i) {
		if (edit_string[i] == '+')
			res += '-';
		else
			res += normalize(old_code[j++]);
	}

	res += '\n';

	for (size_t i = 0, j = 0; i < edit_string.length(); ++i) {
		if (edit_string[i] == '-')
			res += '-';
		else
			res += normalize(new_code[j++]);
	}

	res += '\n' + edit_string;
	return res;

}

char normalize(const char c) {
	if (c == '\n' || c == '\r' || c == '\t')
		return ' ';
	return c;
}