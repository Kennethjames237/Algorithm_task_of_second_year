#include "treeUtils.hpp"

namespace treeUtils {
	// to review, use a better method like string builder, and better
	// visualization
	std::string treeToString(Node* node) {
		if (!node)
			return "";
		std::string res = "";
		res += std::to_string(node->data);
		res += treeToString(node->left);
		res += std::to_string(node->data);
		res += treeToString(node->right);
		res += std::to_string(node->data);
		return res;
	}


	Node* getRandomNode(Node* node, const float p1, const float p2) {
		if (!node) return nullptr;
		if (rand() % 100 < p1 * 100) return node;
		Node* fromLeft = getRandomNode(node->left, p1, p2);
		Node* fromRight = getRandomNode(node->right, p1, p2);
		if (fromLeft && rand() % 100 < p2 * 100) return fromLeft;
		if (fromRight && rand() % 100 < p2 * 100) return fromRight;
		return node;
	}

	Node* createNode(int data) {
		static uint id = 0;
		Node* newNode = new Node();
		newNode->id = id++;
		newNode->data = data;
		newNode->left = nullptr;
		newNode->right = nullptr;
		return newNode;
	}

	Node* getNode(Node* node, uint id) {
		if (!node) return nullptr;
		if (node->id == id) return node;
		Node* temp = getNode(node->left, id);
		return (temp) ? temp : getNode(node->right, id);
	}

	Node* randomTree(int depth, const float p) {
		if (depth == 0)
			return nullptr;
		Node* node = createNode(rand() % MAX_NUMBER);
		if (rand() % 100 < p * 100)
			node->left = randomTree(depth - 1, p);
		if (rand() % 100 < p * 100)
			node->right = randomTree(depth - 1, p);
		return node;
	}

	Node* simpleTree(int depth) { return randomTree(depth, 0); }

	void deleteTree(Node* root) {
		if (root) {
			deleteTree(root->left);
			deleteTree(root->right);
			delete root;
		}
	}

	Node* flip(Node* root) {
		if (!root)
			return root;
		Node* temp = root->right;
		root->right = root->left;
		root->left = temp;
		flip(root->right);
		flip(root->left);
		return root;
	}

	bool isBalanced(Node* root) {
		//base case
		if (root == nullptr) return true;
		//return the height of the left subtree 
		int left = height(root->left);
		//return the height of the right subtree 
		int right = height(root->right);
		if (abs(left - right) > 1) return false;
		return isBalanced(root->right) && isBalanced(root->left);
	}

	int max(int a, int b) {
		if (a > b)
			return a;
		else
			return b;
	}

	int height(Node* root) {
		if (!root)
			return -1;
		int left = height(root->left);
		int right = height(root->right);
		return 1 + max(left, right);
	}

	void depth(Node* root, int i, std::ostream& out) {
		if (!root)
			return;
		out << "Depth of node " << root << " is " << i << std::endl;
		depth(root->left, i + 1, out);
		depth(root->right, i + 1, out);
	}

	bool isComplete(Node* root) {
		if (!root) return true;
		
		// Usa un array per simulare una coda (dimensione massima basata sull'altezza)
		int h = height(root);
		int maxNodes = pow(2, h + 1) - 1;
		Node** queue = new Node*[maxNodes];
		int front = 0, rear = 0;
		
		queue[rear++] = root;
		bool foundNull = false;
		
		while (front < rear) {
			Node* current = queue[front++];
			
			if (current == nullptr) {
				foundNull = true;
			} else {
				// Se abbiamo già trovato un nodo null e ora troviamo un nodo non-null,
				// l'albero non è completo
				if (foundNull) {
					delete[] queue;
					return false;
				}
				
				queue[rear++] = current->left;
				queue[rear++] = current->right;
			}
		}
		
		delete[] queue;
		return true;
	}

	std::ostream& operator<<(std::ostream& out, const Node* node) {
		if (node) out << "(value:" << node->data << ", id:" << node->id << ")";
		else out << "(nullptr)";
		return out;
	}

	uint LowestCommonAncestor(Node* node, const uint x, const uint y) {
		if (!node) return 0;
		if (node->id == x || node->id == y) return node->id;
		uint left = LowestCommonAncestor(node->left, x, y);
		uint right = LowestCommonAncestor(node->right, x, y);
		if (left != 0 && right != 0)
			return node->id;
		if (left != 0)
			return left;
		return right;
		//return (left) ? left : right;
	}

} // namespace treeUtils

