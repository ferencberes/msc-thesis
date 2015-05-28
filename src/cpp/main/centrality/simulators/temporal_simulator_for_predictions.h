#ifndef TEMPORAL_SIMULATOR_FOR_PREDICTIONS_H
#define TEMPORAL_SIMULATOR_FOR_PREDICTIONS_H

#include "temporal_simulator.h"
#include <ctime>
#include <map>

using namespace lemon;

class TemporalSimulatorForPredictions: public TemporalSimulator {
public:
	TemporalSimulatorForPredictions(ListDigraph &graph,
			ListDigraph::NodeMap<NodeId> &id,
			ListDigraph::ArcMap<long> &mention_time, EdgeWeightMap &edge_weights, char* output_folder,
			string prediction_dir, long num_of_predictions) :
			TemporalSimulator(graph, id, mention_time, edge_weights, output_folder), _num_of_pred_edges(
					num_of_predictions), _preproc_prediction_dir(
					prediction_dir), _is_predicted_edge(_graph, false) {

		_logger->info(
				"TemporalSimulatorForPredictions: initialization STARTED.)");

		clock_t begin = clock();
		_reserved_edges.resize(_num_of_pred_edges);
		_num_of_nodes = countNodes(_graph);
		_node_vector.resize(_num_of_nodes);
		long pos = 0;
		for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
			_position_map.insert(std::pair<NodeId, long>(_id[n], pos));
			_node_vector[pos] = n;
			pos++;
		}
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		_logger->infoStream()
				<< "TemporalSimulatorForPredictions: initialization FINISHED in "
				<< elapsed_secs << " sec.";

	}

	void setConfigForSimulation(int argc, char* argv[]);
	void filter_current_interval_graph(int interval_count, long actualTime,
			NodeFilter &current_nodes, EdgeFilter &current_edges,
			MultiEdgeCounter &current_edge_counter,
			GlobalEdgeCounter &global_edge_counter);
	void loadPredictionForInterval(int interval_counter);
	void showUsage();

private:
	long _num_of_nodes;
	long _num_of_pred_edges;
	string _preproc_prediction_dir;
	EdgeFilter _is_predicted_edge;
	std::vector<ListDigraph::Arc> _reserved_edges;
	std::vector<ListDigraph::Node> _node_vector;
	std::map<NodeId, long> _position_map;
};

#endif
