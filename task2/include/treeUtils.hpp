#ifndef TREE_UTILS_HPP
#define TREE_UTILS_HPP

#include <cmath>
#include <iostream>
#include <string>

namespace treeUtils {

	struct Node {
		int data;
		uint id;
		Node* left;
		Node* right;
	};

	constexpr int MAX_NUMBER = 100;
  
	Node* createNode(int);
	Node* randomTree(int, const float = 0.5);
	Node* simpleTree(int);
	void deleteTree(Node*);
	std::string treeToString(Node*);
	int max(int, int);
	int height(Node*);
	std::ostream& operator<<(std::ostream&, const Node*);
	Node* getRandomNode(Node*, const float p1 = 0.1, const float p2 = 0.7);
	Node* getNode(Node*, const uint);

	// task

	Node* flip(Node*);
	void depth(Node*, int = 0, std::ostream& = std::cout);
	bool isBalanced(Node*);
	int getMaxId(Node* root);
	bool isComplete(Node*);
	uint LowestCommonAncestor(Node*, const uint, const uint);
}; // namespace treeUtils

#endif
