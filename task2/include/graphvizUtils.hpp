
#ifndef GRAPHVIZ_UTILS_HPP
#define GRAPHVIZ_UTILS_HPP

#include "treeUtils.hpp"
#include <gvc.h>

namespace gvUtils {

	void exportToDot(treeUtils::Node*, const std::string&);

	namespace internal {
		Agraph_t* buildAgraph(treeUtils::Node*);
		void createAgEdge(Agraph_t*, Agnode_t*, Agnode_t*, bool = true);
		Agnode_t* createAgNode(Agraph_t*, uint, std::string);
		Agraph_t* initGraph();
	}; // namespace internal
}; // namespace gvUtils

#endif
