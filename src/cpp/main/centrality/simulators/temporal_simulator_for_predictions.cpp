#include "temporal_simulator_for_predictions.h"
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace lemon;

void TemporalSimulatorForPredictions::setConfigForSimulation(int argc,
		char* argv[]) {
	_measureSet.clear();
	getTimeBounds();
	if (argc > 12) {
		for (int i = 13; i < argc; ++i) {
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
		setConfigVariables(atol(argv[5]), atol(argv[6]), atoi(argv[7]),
				atof(argv[8]), atoi(argv[9]),
				(string2bool(argv[10]) ? true : false), atoi(argv[11]),
				atoi(argv[12]), _runDegree, _runBeta, _runPageRank, _runLoop,
				_runSalsa, _runHarmonic);
	} else {
		showUsage();
	}
}

void TemporalSimulatorForPredictions::filter_current_interval_graph(
		int interval_count, long actualTime, NodeFilter &current_nodes,
		EdgeFilter &current_edges, MultiEdgeCounter &current_edge_counter,
		GlobalEdgeCounter &global_edge_counter) {

	// there is no prediction for the first interval
	if (interval_count != 0) {
		loadPredictionForInterval(interval_count);
	}

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
				if ((_mention_time[out] >= _startTime) && ((actualTime > _mention_time[out]
						&& _mention_time[out] >= actualTime - counter * _delta
						&& !_is_predicted_edge[out])
						|| (actualTime <= _mention_time[out]
								&& _is_predicted_edge[out]))) {
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
				} else {
					// set default edge weight
					_edge_weights[out] = 1.0;
					// this is valid: real edges of current interval are false from default! Will be true in the next interval.
					if (_centrality_prev_interval_count != -1) {
						// exclude previous edges if it is configured
						current_edges[out] = false;
					}
				}
			}
		}
	}
}

void TemporalSimulatorForPredictions::loadPredictionForInterval(
		int interval_counter) {
	ostringstream convert;
	convert << interval_counter;
	string predFileName = _preproc_prediction_dir + "/prediction_edge_list_"
			+ convert.str();
	std::ifstream pred_file(predFileName.c_str());

	long edge_pos = 0;
	_logger->info(
			"TemporalSimulatorForPredictions: loading preprocessed prediction file STARTED");
	clock_t begin = clock();
	for (std::string line; getline(pred_file, line);) {
		long token_pos = 0;
		NodeId source_id;
		NodeId target_id;
		std::stringstream ss(line);
		std::string token;
		while (ss >> token) {
			if (token_pos == 0) {
				source_id = atol(token.c_str());
			} else {
				if (edge_pos >= _num_of_pred_edges) {
					_logger->errorStream()
							<< "Predefined number of predicted edges for interval "
							<< interval_counter + 1
							<< " is smaller than the number of edges in the preprocessed file.";
					abort();
				}
				// delete former interval prediction (first only in the second interval)
				if (interval_counter > 1) {
					_graph.erase(_reserved_edges[edge_pos]);
				}

				target_id = atol(token.c_str());
				// add current interval prediction
				_reserved_edges[edge_pos] = _graph.addArc(
						_node_vector[_position_map[source_id]],
						_node_vector[_position_map[target_id]]);

				_mention_time[_reserved_edges[edge_pos]] = _startTime
						+ interval_counter * _delta;
				_edge_weights[_reserved_edges[edge_pos]] = 1.0;

				_is_predicted_edge[_reserved_edges[edge_pos]] = true;
				//std::cout << "Adding edge " << edge_pos << ".: " << source_id << "->" << target_id << std::endl;
				edge_pos++;
			}
			token_pos++;
		}
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	_logger->infoStream()
			<< "TemporalSimulatorForPredictions: loading preprocessed prediction file was FINISHED in "
			<< elapsed_secs << " sec.";
	pred_file.close();
}

void TemporalSimulatorForPredictions::showUsage() {
	_logger->warnStream()
			<< "Usage: <input_lgf> <output_folder> <prediction_folder> <num_of_predicted_edges_in_interval> <start_time> <delta> <centrality_prev_interval_count> <combine_factor> <graph_stat_prev_interval_count> <enable_multi_edges> <max_num_of_steps> <topK> OPTIONS:(-beta, -degree, -pr, -salsa, -loop, -harmonic)";
}
