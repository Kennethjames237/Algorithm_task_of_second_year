//Nna minkousse Kenneth James
//Belli Andrea
//Premori Samuele

#include <iostream>
#include <sstream>

#include "graphvizUtils.hpp"
#include "treeUtils.hpp"

constexpr const int MAX_DEPTH = 3;

void propertiesTest(char* a, char* b);

int main(int argc, char* argv[]) {
	srand(unsigned(time(nullptr)));
	if (argc != 3)
		return 1;
	propertiesTest(argv[1], argv[2]);
}

void propertiesTest(char* a, char* b) {
	using namespace treeUtils;
	using namespace gvUtils;
	Node* root = randomTree(MAX_DEPTH);
	exportToDot(root, a);
	flip(root);
	exportToDot(root, b);
	flip(root);
	depth(root, 0, std::cout);
	std::cout << std::boolalpha;
	std::cout << treeToString(root) << std::endl;
	std::cout << "Is balanced: " << isBalanced(root) << std::endl;
	std::cout << "Is complete: " << isComplete(root) << std::endl;
	Node* x = getRandomNode(root);
	Node* y = getRandomNode(root);
	uint ancestorId = LowestCommonAncestor(root, x->id, y->id);
	std::cout << "Lowest common ancestor for " << x << y << " : " << getNode(root, ancestorId) << std::endl;
	deleteTree(root);
}
