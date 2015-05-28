#include "temporal_simulator.h"
#include <limits.h>
#include <stdlib.h>
#include <math.h>       /* ceil */

using namespace lemon;

void TemporalSimulator::setLogLevel(bool enabled) {
	enabled ?
			_logger->setPriority(log4cpp::Priority::INFO) :
			_logger->setPriority(log4cpp::Priority::ERROR);
	_console_logging_enabled = enabled;
}

void TemporalSimulator::getGraphInfo() {
	_logger->infoStream() << "Number of vertices in the data: " << countNodes(_graph);
	_logger->infoStream() << "Number of edges in the data: " << countArcs(_graph);
}

void TemporalSimulator::getTimeInfo() {
	getTimeBounds();
	double time_diff = _stopTime - _minTime;
	double max_number_of_days = time_diff / 86400;
	_logger->infoStream() << "min time: " << _minTime;
	_logger->infoStream() << "max time: " << _stopTime;
	_logger->infoStream() << "Number of days in the data: "
			<< ceil(max_number_of_days);
}

void TemporalSimulator::getTimeBounds() {
	_minTime = INT_MAX;
	_stopTime = 0;

	for (ListDigraph::ArcIt e(_graph); e != INVALID; ++e) {
		if (_mention_time[e] < _minTime) {
			_minTime = _mention_time[e];
		}
		if (_mention_time[e] > _stopTime) {
			_stopTime = _mention_time[e];
		}
	}
}

void TemporalSimulator::setConfigForSimulation(int argc, char* argv[]) {
	printInterfaceError();
}

void TemporalSimulator::setConfigVariables(long startTime, long delta,
		int centrality_lookback_count, float combine_factor,
		int graph_stat_lookback_count, bool enable_multi_edges, int numOfSteps,
		int topKNumber, bool runDegree, bool runBeta, bool runPageRank,
		bool runLoop, bool runSalsa, bool runHarmonic) {
	getTimeBounds();
	_startTime = startTime;
	_delta = delta;
	_combine_factor = combine_factor;
	_centrality_prev_interval_count = centrality_lookback_count;
	_graph_stat_prev_interval_count = graph_stat_lookback_count;
	if (_centrality_prev_interval_count == -1) {
		if (_graph_stat_prev_interval_count != -1) {
			_logger->error(
					"graph_stat_prev_interval_count must equal -1 if centrality_prev_interval_count=-1!");
			_valid_config = false;
			showUsage();
			abort();
		}
	} else {
		if (_graph_stat_prev_interval_count != -1
				&& _graph_stat_prev_interval_count
						< _centrality_prev_interval_count) {
			_logger->error(
					"graph_stat_prev_interval_count must be -1 or at least the value of centrality_prev_interval_count!");
			_valid_config = false;
			showUsage();
			abort();
		}
	}

	_enable_multi_edges = enable_multi_edges;
	_numOfSteps = numOfSteps;
	_topKNumber = topKNumber;

	_measureSet.clear();
	if (runDegree) {
		_runDegree = true;
		_measureSet.push_back("in_degree");
		_measureSet.push_back("out_degree");
	}
	if (runBeta) {
		_runBeta = true;
		_measureSet.push_back("beta");
	}
	if (runLoop) {
		_runLoop = true;
		_measureSet.push_back("loop");
	}
	if (runPageRank) {
		_runPageRank = true;
		_measureSet.push_back("pagerank");
	}
	if (runSalsa) {
		_runSalsa = true;
		_measureSet.push_back("salsa_auth");
		_measureSet.push_back("salsa_hub");
	}
	if (runHarmonic) {
		_runHarmonic = true;
		_measureSet.push_back("harmonic");
	}

	_valid_config = true;
}

void TemporalSimulator::simulate() {
	if (_valid_config) {
		_logger->info("Parameters are valid.");

		_current_edge_changes.clear();
		_current_node_changes.clear();
		_new_hom_edge_changes.clear();
		_new_edge_changes.clear();
		_new_node_changes.clear();
		_previous_edge_changes.clear();
		_previous_node_changes.clear();
		_deleted_edge_changes.clear();
		_deleted_node_changes.clear();

		EdgeFilter current_edges(_graph, false);
		NodeFilter current_nodes(_graph, false);
		NodeFilter prev_nodes(_graph, false);
		TemporalSubGraph subg(_graph, current_nodes, current_edges);

		MultiEdgeCounter curr_edge_counter(_graph, counter_map());
		MultiEdgeCounter prev_edge_counter(_graph, counter_map());

		GlobalEdgeCounter global_edge_counter(_graph, first_mention_map());
		GlobalEdgeTimeQueue global_edge_time_queue;

		TemporalGraphStatCounter stat_counter(_enable_multi_edges, _graph,
				current_nodes, prev_nodes, curr_edge_counter,
				prev_edge_counter);

		TemporalCentralityGraph cg(subg, _id, _edge_weights, _topKNumber,
				_console_logging_enabled);

		CentralityJsonWriter json_writer(_output_folder, cg, _measureSet,
				stat_counter, _console_logging_enabled);
		RecsysEdgeWriter recsys_writer(_output_folder, _enable_multi_edges,
				_graph, _id, _console_logging_enabled);

		if (_writers_enabled) {
			json_writer.writeHeader(_startTime, _delta,
					_centrality_prev_interval_count,
					_graph_stat_prev_interval_count, _enable_multi_edges,
					_numOfSteps, _topKNumber);
		}

		long actualTime = _startTime;
		_logger->infoStream() << "Start time: " << _startTime;
		_logger->infoStream() << "Delta time: " << _delta;
		_logger->infoStream() << "numOfSteps: " << _numOfSteps;

		_logger->info("STARTING temporal simulation...");
		for (int i = 0; i < _numOfSteps; ++i) {
			_logger->info("STARTING interval %d...", i + 1);
			if (i > 0) {
				actualTime += _delta;
				if (actualTime > _stopTime) {
					_logger->warnStream() << "In the " << i + 1
							<< ". step: timeline ended earlier than the predefined steps!";
					break;
				}
			}

			// filter the current and the previous intervals for analysis.
			_logger->info("Filtering graph data for interval %d.", i + 1);
			filter_current_interval_graph(i, actualTime, current_nodes,
					current_edges, curr_edge_counter, global_edge_counter);
			_logger->info("Filtering graph data for previous intervals.");
			filter_prev_interval_graph(actualTime, prev_nodes,
					prev_edge_counter);

			// global_edge_time_queue must be updated  in each time interval
			_logger->info("Filtering graph data for global time queue.");
			filter_edges_for_global_time_queue(global_edge_counter,
					global_edge_time_queue);

			// store graph data changes
			_current_edge_changes.push_back(stat_counter.numOfCurrentEdges());
			_current_node_changes.push_back(stat_counter.numOfCurrentNodes());
			_new_edge_changes.push_back(stat_counter.numOfNewEdges());
			_new_hom_edge_changes.push_back(
					stat_counter.numOfNewHomophilyEdges());
			_new_node_changes.push_back(stat_counter.numOfNewNodes());
			_previous_edge_changes.push_back(stat_counter.numOfPreviousEdges());
			_previous_node_changes.push_back(stat_counter.numOfPreviousNodes());
			_deleted_edge_changes.push_back(stat_counter.numOfDeletedEdges());
			_deleted_node_changes.push_back(stat_counter.numOfDeletedNodes());

			// compute the centrality measures that were configured to be run
			computeCentralityMeasures(i, cg);

			/*
			 // SYSO for temporal edge change testing
			 printEdgeCounterMap(_graph,_id, curr_edge_counter);
			 printEdgeCounterMap(_graph,_id, prev_edge_counter);
			 printNodeMap(_graph,_id, current_nodes);
			 printNodeMap(_graph,_id, prev_nodes);
			 printEdgeMap(_graph,_id, current_edges);
			 */

			if (_writers_enabled) {
				recsys_writer.writeInterval(global_edge_time_queue,
						curr_edge_counter, prev_edge_counter, prev_nodes, i);

				if (actualTime + _delta > _stopTime) {
					// make this interval the last in the json output
					if (_writers_enabled) {
						json_writer.writeInterval(true, i, actualTime,
								actualTime + _delta);
					}
				} else {
					json_writer.writeInterval(i == _numOfSteps - 1, i,
							actualTime, actualTime + _delta);
				}
			}
			_logger->info("FINISHING interval %d...", i + 1);
		}
		_logger->info("temporal simulation FINISHED.");
		if (_writers_enabled) {
			json_writer.close();
		}
	} else {
		_logger->warn(
				"Use setConfigForSimulation() to set the parameters for the simulation!");
	}
}

void TemporalSimulator::filter_edges_for_global_time_queue(
		GlobalEdgeCounter &global_edge_counter,
		GlobalEdgeTimeQueue &global_edge_time_queue) {
	// clear the edge time queue for each interval
	while (!global_edge_time_queue.empty()) {
		global_edge_time_queue.pop();
	}

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (first_mention_map_iter it = global_edge_counter[n].begin();
				it != global_edge_counter[n].end(); ++it) {
			if (it->second != -1) {
				global_edge_time_queue.push(
						recsys_record(it->second, _id[n], _id[it->first], 1,
								0));
				// With this assignment we make sure each edge is added to the queue once
				it->second = -1;
			}
		}
	}
}

void TemporalSimulator::filter_current_interval_graph(int interval_count,
		long actualTime, NodeFilter &current_nodes, EdgeFilter &current_edges,
		MultiEdgeCounter &current_edge_counter,
		GlobalEdgeCounter &global_edge_counter) {
	printInterfaceError();
}

void TemporalSimulator::filter_prev_interval_graph(long actualTime,
		NodeFilter &prev_nodes, MultiEdgeCounter &prev_edge_counter) {

	int counter;
	if (_graph_stat_prev_interval_count != -1) {
		counter = _graph_stat_prev_interval_count;
		for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
			prev_edge_counter[n].clear();
			prev_nodes[n] = false;
		}
	} else {
		counter = 1;
	}

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (ListDigraph::OutArcIt out(_graph, n); out != INVALID; ++out) {
			if (_mention_time[out] < actualTime) {
				if (_mention_time[out] >= actualTime - counter * _delta && _mention_time[out] >= _startTime) {
					// filter previous nodes
					prev_nodes[n] = true;
					prev_nodes[_graph.target(out)] = true;

					if (prev_edge_counter[n].end()
							== prev_edge_counter[n].find(_graph.target(out))) {
						prev_edge_counter[n].insert(
								std::pair<ListDigraph::Node, mention_time_list>(
										_graph.target(out),
										mention_time_list()));
					}
					prev_edge_counter[n][_graph.target(out)].push(
							_mention_time[out]);
				}
			}
		}
	}
}

void TemporalSimulator::computeCentralityMeasures(int iteration_counter,
		TemporalCentralityGraph &cg) {
	// degree maps of the graph must be updated!
	cg.updateDegreeMaps();

	if (cg.numOfEdges() > 0 && cg.numOfVertices() > 0) {
		_logger->info(
				"In interval %d: We have a subgraph with %d nodes and %d edges.",
				iteration_counter + 1, cg.numOfVertices(), cg.numOfEdges());
		if (_runDegree) {
			cg.sortNeighbours();
		}
		if (_runBeta) {
			cg.computeBetaMeasure();
		}
		if (_runLoop) {
			cg.computeLoopEdgeRatio();
		}
		if (_runPageRank) {
			cg.computePageRank(0.85, NUM_OF_ITERATION);
		}
		if (_runSalsa) {
			cg.computeSalsa(NUM_OF_ITERATION);
		}
		if (_runHarmonic) {
			cg.computeHarmonic();
		}
	} else {
		_logger->warn(
				"In interval %d: We have a subgraph with %d nodes and %d edges.",
				iteration_counter + 1, cg.numOfVertices(), cg.numOfEdges());
	}
}

void TemporalSimulator::showUsage() {
	printInterfaceError();
}

void TemporalSimulator::printInterfaceError() {
	_logger->error(
			"TemporalSimulator is only an interface! Use %s or %s instead.",
			"BasicTemporalSimulator", "TemporalSimulatorForPredictions");
}
