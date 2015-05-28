#ifndef TEMPORAL_GRAPH__STAT_COUNTER_H
#define TEMPORAL_GRAPH__STAT_COUNTER_H

#include "temporal_centrality_utils.h"

using namespace lemon;

class TemporalGraphStatCounter {
public:
	TemporalGraphStatCounter(bool enable_multi_edges, ListDigraph &g,
			NodeFilter &curr_n, NodeFilter &prev_n, MultiEdgeCounter &c_e_c,
			MultiEdgeCounter &p_e_c) :
			_enable_multi_edges(enable_multi_edges), _graph(g), _current_nodes(
					curr_n), _prev_nodes(prev_n), _current_edge_counter(c_e_c), _prev_edge_counter(
					p_e_c) {
	}
	;

	long numOfCurrentEdges();
	long numOfNewEdges();
	long numOfNewHomophilyEdges();
	long numOfPreviousEdges();
	long numOfDeletedEdges();
	long numOfCurrentNodes();
	long numOfNewNodes();
	long numOfPreviousNodes();
	long numOfDeletedNodes();

private:
	bool _enable_multi_edges;
	ListDigraph &_graph;
	NodeFilter &_current_nodes;
	NodeFilter &_prev_nodes;
	MultiEdgeCounter &_current_edge_counter;
	MultiEdgeCounter &_prev_edge_counter;

};
#endif
