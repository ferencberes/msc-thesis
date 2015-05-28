#include "basic_temporal_simulator.h"

using namespace lemon;

void BasicTemporalSimulator::setConfigForSimulation(int argc, char* argv[]) {
	_measureSet.clear();
	getTimeBounds();
	if (argc > 10) {
		for (int i = 11; i < argc; ++i) {
			string param = string(argv[i]);
			if (param == "-degree") {
				_runDegree = true;
			}
			if (param == "-beta") {
				_runBeta = true;
			}
			if (param == "-loop") {
				_runLoop = true;
			}
			if (param == "-pr") {
				_runPageRank = true;
			}
			if (param == "-salsa") {
				_runSalsa = true;
			}
			if (param == "-harmonic") {
				_runHarmonic = true;
			}
		}
		setConfigVariables(atol(argv[3]), atol(argv[4]), atoi(argv[5]),
				atof(argv[6]), atoi(argv[7]),
				(string2bool(argv[8]) ? true : false), atoi(argv[9]),
				atoi(argv[10]), _runDegree, _runBeta, _runPageRank, _runLoop,
				_runSalsa, _runHarmonic);
	} else {
		showUsage();
	}
}

void BasicTemporalSimulator::filter_current_interval_graph(int interval_count,
		long actualTime, NodeFilter &current_nodes, EdgeFilter &current_edges,
		MultiEdgeCounter &current_edge_counter,
		GlobalEdgeCounter &global_edge_counter) {

	int counter;
	// exclude previous nodes if it is configured
	if (_centrality_prev_interval_count != -1) {
		counter = _centrality_prev_interval_count;
		for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
			current_edge_counter[n].clear();
			current_nodes[n] = false;
		}
	} else {
		counter = 0;
	}

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (ListDigraph::OutArcIt out(_graph, n); out != INVALID; ++out) {
			if (_mention_time[out] < actualTime + _delta) {
				if (_mention_time[out] >= actualTime - counter * _delta && _mention_time[out] >= _startTime) {

					// filter active nodes
					current_nodes[n] = true;
					current_nodes[_graph.target(out)] = true;

					if (current_edge_counter[n].end()
							== current_edge_counter[n].find(
									_graph.target(out))) {
						current_edge_counter[n].insert(
								std::pair<ListDigraph::Node, mention_time_list>(
										_graph.target(out),
										mention_time_list()));
						// filter active edges
						current_edges[out] = true;
					} else {
						if (_enable_multi_edges) {
							current_edges[out] = true;
						}
					}

					// set edge weights for current interval edges except default combine_factor (-0.1)
					if (_combine_factor != -1.0 && current_edges[out]) {
						if (_mention_time[out] < actualTime) {
							_edge_weights[out] = _combine_factor;
						} else {
							_edge_weights[out] = (1 - _combine_factor);
						}
					}

					current_edge_counter[n][_graph.target(out)].push(
							_mention_time[out]);

					// TODO: megnézni, hogy legális-e amit a centrality visszatekintéssel csinál a globális toplist!!!
					// filter whether this edge has ever existed or not
					if (global_edge_counter[n].find(_graph.target(out))
							== global_edge_counter[n].end()) {
						global_edge_counter[n].insert(
								std::pair<ListDigraph::Node, long>(
										_graph.target(out),
										_mention_time[out]));
					} else {
						// we search for the first mention
						long former_value = global_edge_counter[n].find(
								_graph.target(out))->second;
						if (former_value > _mention_time[out]
								&& former_value != -1) {
							global_edge_counter[n].find(_graph.target(out))->second =
									_mention_time[out];
						}
					}

				} else {
					// set default edge weight
					_edge_weights[out] = 1.0;
					if (_centrality_prev_interval_count != -1) {
						// exclude previous edges if it is configured
						current_edges[out] = false;
					}
				}
			}
		}
	}
}

void BasicTemporalSimulator::showUsage() {
	_logger->warnStream()
			<< "Usage: <input_lgf> <output_folder> <start_time> <delta> <centrality_prev_interval_count> <combine_factor> <graph_stat_prev_interval_count> <enable_multi_edges> <max_num_of_steps> <topK> OPTIONS:(-beta, -degree, -pr, -salsa, -loop, -harmonic)";
}
