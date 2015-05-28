#include "temporal_centrality_utils.h"

using namespace lemon;

bool string2bool(const std::string & v) {
	return !v.empty()
			&& (strcasecmp(v.c_str(), "true") == 0 || atoi(v.c_str()) != 0);
}

std::string recsys_record::print() {
	std::stringstream ss;
	ss << mention_time << " " << source << " " << target << " " << value
			<< " " << eval;
	return ss.str();
}

std::string recsys_record::printHomophily() {
	std::stringstream ss;
	ss << mention_time << " " << source << " " << target;
	return ss.str();
}

bool operator<(const recsys_record& a, const recsys_record& b) {
	return a.mention_time > b.mention_time;
}

void printNodeMap(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
		NodeFilter &map) {
	for (ListDigraph::NodeIt n(graph); n != INVALID; ++n) {
		std::cout << id[n] << ":" << map[n] << std::endl;
	}
}

void printEdgeMap(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
		EdgeFilter &map) {
	for (ListDigraph::ArcIt a(graph); a != INVALID; ++a) {
		std::cout << id[graph.source(a)] << "," << id[graph.target(a)] << ":"
				<< map[a] << std::endl;
	}
}

void printEdgeCounterMap(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
		MultiEdgeCounter &counter_map) {
	for (ListDigraph::NodeIt n(graph); n != INVALID; ++n) {
		for (counter_map_iterator it = counter_map[n].begin();
				it != counter_map[n].end(); ++it) {
			std::cout << id[n] << "->" << id[it->first] << ":"
					<< it->second.size() << "\n";
		}
	}

}
