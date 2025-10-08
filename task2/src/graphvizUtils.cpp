#include "graphvizUtils.hpp"
#include <fstream>
#include <functional>

namespace gvUtils {

	void exportToDot(treeUtils::Node* root, const std::string& filename) {
		GVC_t* gvc = gvContext();
		Agraph_t* graph = internal::buildAgraph(root);
		gvLayout(gvc, graph, "dot");
		FILE* fp = fopen(filename.c_str(), "w");
		gvRender(gvc, graph, "dot", fp);
		gvFreeLayout(gvc, graph);
		agclose(graph);
		gvFreeContext(gvc);
		fclose(fp);
	}

	namespace internal {

		Agraph_t* buildAgraph(treeUtils::Node* root) {
			Agraph_t* graph = initGraph();
			std::function<Agnode_t*(treeUtils::Node*)> build =
				[&](treeUtils::Node* node) {
					Agnode_t* anode = createAgNode(
						graph, node->id, ::std::to_string(node->data));
					if (node->left)
						createAgEdge(graph, anode, build(node->left), true);
					if (node->right)
						createAgEdge(graph, anode, build(node->right), false);
					return anode;
				};
			build(root);
			return graph;
		}

		void createAgEdge(Agraph_t* graph, Agnode_t* a, Agnode_t* b,
						  bool left) {
			Agedge_t* edge = agedge(graph, a, b, (char*)"", 1);
			agsafeset(edge, const_cast<char*>("side"),
					  const_cast<char*>((left) ? "left" : "right"), "");
		}

		Agnode_t* createAgNode(Agraph_t* graph, uint id, ::std::string value) {
			std::string id_str = std::to_string(id);
			std::string label = "value=" + value + "\nid=" + id_str;
			Agnode_t* node = agnode(graph, const_cast<char*>(id_str.c_str()), 1);
			agsafeset(node, const_cast<char*>("label"),
					  const_cast<char*>(label.c_str()), "");
			return node;
		}

		Agraph_t* initGraph() {
			Agraph_t* graph =
				agopen(const_cast<char*>("BinaryTree"), Agstrictundirected, 0);
			agattr(graph, AGEDGE, const_cast<char*>("arrowhead"),
				   const_cast<char*>("none"));
			agattr(graph, AGEDGE, const_cast<char*>("splines"),
				   const_cast<char*>("line"));
			agattr(graph, AGNODE, const_cast<char*>("width"),
				   const_cast<char*>("2"));
			agattr(graph, AGNODE, const_cast<char*>("shape"),
				   const_cast<char*>("circle"));
			agattr(graph, AGNODE, const_cast<char*>("fixedsize"),
				   const_cast<char*>("true"));
			agattr(graph, AGNODE, const_cast<char*>("fontname"),
				   const_cast<char*>("monospace"));
			return graph;
		}

	}; // namespace internal
}; // namespace gvUtils
