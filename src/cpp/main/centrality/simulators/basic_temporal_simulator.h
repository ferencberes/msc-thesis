#ifndef BASIC_TEMPORAL_SIMULATOR_H
#define BASIC_TEMPORAL_SIMULATOR_H

#include "temporal_simulator.h"

using namespace lemon;

class BasicTemporalSimulator: public TemporalSimulator {
public:
	BasicTemporalSimulator(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
			ListDigraph::ArcMap<long> &mention_time, EdgeWeightMap &edge_weights, char* output_folder) :
				TemporalSimulator(graph, id, mention_time, edge_weights, output_folder)
			{}

	void setConfigForSimulation(int argc, char* argv[]);
	void filter_current_interval_graph(int interval_count, long actualTime,
			NodeFilter &current_nodes, EdgeFilter &current_edges,
			MultiEdgeCounter &current_edge_counter,
			GlobalEdgeCounter &global_edge_counter);
	void showUsage();
};

#endif
