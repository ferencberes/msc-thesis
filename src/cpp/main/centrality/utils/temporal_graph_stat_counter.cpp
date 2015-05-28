#include "temporal_graph_stat_counter.h"

using namespace lemon;

long TemporalGraphStatCounter::numOfCurrentEdges() {
	long delta = 0;

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (counter_map_iterator it = _current_edge_counter[n].begin();
				it != _current_edge_counter[n].end(); ++it) {
			if (!it->second.empty()) {
				delta += (_enable_multi_edges ? it->second.size() : 1);
			}
		}
	}
	//std::cout << "current edges: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfNewEdges() {
	long delta = 0;

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (counter_map_iterator it = _current_edge_counter[n].begin();
				it != _current_edge_counter[n].end(); ++it) {
			if (!it->second.empty()
					&& _prev_edge_counter[n].end()
							== _prev_edge_counter[n].find(it->first)) {
				delta += (_enable_multi_edges ? it->second.size() : 1);

			}
		}
	}
	//std::cout << "new edges: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfNewHomophilyEdges() {
	long delta = 0;

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (counter_map_iterator it = _current_edge_counter[n].begin();
				it != _current_edge_counter[n].end(); ++it) {
			if (!it->second.empty()
					&& _prev_edge_counter[n].end()
							== _prev_edge_counter[n].find(it->first)
					&& _prev_nodes[n] && _prev_nodes[it->first]) {
				delta += (_enable_multi_edges ? it->second.size() : 1);
			}
		}
	}
	//std::cout << "hom edges: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfPreviousEdges() {
	long delta = 0;

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (counter_map_iterator it = _prev_edge_counter[n].begin();
				it != _prev_edge_counter[n].end(); ++it) {
			if (!it->second.empty()) {
				delta += (_enable_multi_edges ? it->second.size() : 1);
			}
		}
	}
	//std::cout << "previous edges: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfDeletedEdges() {
	long delta = 0;

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (counter_map_iterator it = _prev_edge_counter[n].begin();
				it != _prev_edge_counter[n].end(); ++it) {
			if (!it->second.empty()
					&& _current_edge_counter[n].end()
							== _current_edge_counter[n].find(it->first)) {
				delta += (_enable_multi_edges ? it->second.size() : 1);
			}
		}
	}
	//std::cout << "deleted edges: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfCurrentNodes() {
	long delta = 0;
	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		if (_current_nodes[n]) {
			delta++;
		}
	}
	//std::cout << "current nodes: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfNewNodes() {
	long delta = 0;
	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		if ((_current_nodes[n] && !_prev_nodes[n])) {
			delta++;
		}
	}
	//std::cout << "new nodes: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfPreviousNodes() {
	long delta = 0;
	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		if ((_prev_nodes[n])) {
			delta++;
		}
	}
	//std::cout << "previous nodes: " << delta << std::endl;

	return delta;
}

long TemporalGraphStatCounter::numOfDeletedNodes() {
	long delta = 0;
	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		if ((_prev_nodes[n] && !_current_nodes[n])) {
			delta++;
		}
	}
	//std::cout << "deleted nodes: " << delta << std::endl;

	return delta;
}
