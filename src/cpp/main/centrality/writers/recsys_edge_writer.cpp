#include "recsys_edge_writer.h"

void RecsysEdgeWriter::setLogLevel(bool enabled) {
	enabled ?
			_logger->setPriority(log4cpp::Priority::INFO) :
			_logger->setPriority(log4cpp::Priority::ERROR);
	_console_logging_enabled = enabled;
}

void RecsysEdgeWriter::writeInterval(GlobalEdgeTimeQueue &partial_global_edges,
		MultiEdgeCounter &current_interval_edges,
		MultiEdgeCounter &previous_interval_edges, NodeFilter &prev_nodes,
		int interval_counter) {
	writeEdgesOfGlobalTimeQueue(partial_global_edges, interval_counter);
	writeEdgesOfCurrentInterval(current_interval_edges, interval_counter);
	writeHomophilyEdgesOfCurrentInterval(current_interval_edges,
			previous_interval_edges, prev_nodes, interval_counter);
}

void RecsysEdgeWriter::writeEdgesOfGlobalTimeQueue(
		GlobalEdgeTimeQueue &partial_queue, int interval_counter) {
	ofstream _outputFile;
	stringstream ss;
	ss << interval_counter;
	string outputFileName = _edgeOutputFolderName + "/global_edges_" + ss.str();

	_outputFile.open(outputFileName.c_str(), ios::out);

	GlobalEdgeTimeQueue queue_copy(partial_queue);

	while (!queue_copy.empty()) {
		recsys_record rec = queue_copy.top();
		_outputFile << rec.print() << endl;
		queue_copy.pop();
	}
	_outputFile.close();
	_logger->info(
			"RecsysEdgeWriter: global edge data in interval %d was written to %s file.",
			interval_counter + 1, outputFileName.c_str());
}

void RecsysEdgeWriter::writeEdgesOfCurrentInterval(MultiEdgeCounter &current,
		int interval_counter) {
	ofstream _outputFile;
	stringstream ss;
	ss << interval_counter;
	string outputFileName = _edgeOutputFolderName + "/interval_edges_"
			+ ss.str();

	_outputFile.open(outputFileName.c_str(), ios::out);

	// edges will be printed to output in mention-time order
	ordered_edges_list ordered_edges;

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (counter_map_iterator it = current[n].begin();
				it != current[n].end(); ++it) {

			if (!it->second.empty()) {
				if (_enable_multiple_edges) {
					// all mentions of an edge will be printed
					mention_time_list copy_list(it->second);
					while (!copy_list.empty()) {
						ordered_edges.push(
								recsys_record(copy_list.top(), _id[n],
										_id[it->first], 1, 0));
						copy_list.pop();
					}

				} else {
					// only the first mention of an edge will be printed
					ordered_edges.push(
							recsys_record(it->second.top(), _id[n],
									_id[it->first], 1, 0));
				}
			}
		}
	}

	// now the edges will be printed in order
	while (!ordered_edges.empty()) {
		recsys_record rec = ordered_edges.top();
		_outputFile << rec.print() << endl;
		ordered_edges.pop();
	}
	_outputFile.close();
	_logger->info(
			"RecsysEdgeWriter: current edge data in interval %d was written to %s file.",
			interval_counter + 1, outputFileName.c_str());
}

void RecsysEdgeWriter::writeHomophilyEdgesOfCurrentInterval(
		MultiEdgeCounter &current, MultiEdgeCounter &previous,
		NodeFilter &prev_nodes, int interval_counter) {
	ofstream _outputFile;
	stringstream ss;
	ss << interval_counter;
	string outputFileName = _edgeOutputFolderName + "/interval_homophily_edges_"
			+ ss.str();

	_outputFile.open(outputFileName.c_str(), ios::out);

	// edges will be printed to output in mention-time order
	ordered_edges_list ordered_edges;

	for (ListDigraph::NodeIt n(_graph); n != INVALID; ++n) {
		for (counter_map_iterator it = current[n].begin();
				it != current[n].end(); ++it) {

			// homophily edge condition
			if (!it->second.empty()
					&& previous[n].end() == previous[n].find(it->first)
					&& prev_nodes[n] && prev_nodes[it->first]) {

				if (_enable_multiple_edges) {
					// all mentions of an edge will be printed
					mention_time_list copy_list(it->second);
					while (!copy_list.empty()) {
						ordered_edges.push(
								recsys_record(interval_counter, _id[n],
										_id[it->first], 1, 0));
						copy_list.pop();
					}

				} else {
					// only the first mention of an edge will be printed
					ordered_edges.push(
							recsys_record(interval_counter, _id[n],
									_id[it->first], 1, 0));
				}
			}
		}
	}

	// now the edges will be printed in order
	while (!ordered_edges.empty()) {
		recsys_record rec = ordered_edges.top();
		_outputFile << rec.printHomophily() << endl;
		ordered_edges.pop();
	}
	_outputFile.close();
	_logger->info(
			"RecsysEdgeWriter: current homophily edge data in interval %d was written to %s file.",
			interval_counter + 1, outputFileName.c_str());
}
