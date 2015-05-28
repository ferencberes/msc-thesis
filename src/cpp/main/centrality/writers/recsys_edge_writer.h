#ifndef RECSYS_EDGE_WRITER_H
#define RECSYS_EDGE_WRITER_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "../graphs/temporal_centrality_graph.h"

using namespace std;

class RecsysEdgeWriter {
public:
	RecsysEdgeWriter(string output_folder, bool e_m_edges, ListDigraph &graph,
			ListDigraph::NodeMap<NodeId> &id, bool enable_logging) :
			_enable_multiple_edges(e_m_edges), _graph(graph), _id(id), _console_logging_enabled(
					enable_logging) {
		log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
		layout->setConversionPattern("%d [%p] %m%n");
		log4cpp::Appender *consol_appender = new log4cpp::OstreamAppender(
				"console", &std::cout);
		consol_appender->setLayout(layout);
		_logger = &log4cpp::Category::getInstance(
				std::string("RecsysEdgeWriter"));
		_logger->addAppender(consol_appender);
		setLogLevel(_console_logging_enabled);

		_edgeOutputFolderName = output_folder + "/edges_in_intervals";
		// output folder was created
		mkdir(_edgeOutputFolderName.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
		_logger->info(
				"RecsysEdgeWriter: interval edge data will be written to %s folder.",
				_edgeOutputFolderName.c_str());

	}

	void setLogLevel(bool enabled);
	void writeInterval(GlobalEdgeTimeQueue &partial_global_edges,
			MultiEdgeCounter &interval_edges,
			MultiEdgeCounter &previous_interval_edges, NodeFilter &prev_nodes,
			int interval_counter);
	void writeEdgesOfGlobalTimeQueue(GlobalEdgeTimeQueue &partial_queue,
			int interval_counter);
	void writeEdgesOfCurrentInterval(MultiEdgeCounter &current,
			int interval_counter);
	void writeHomophilyEdgesOfCurrentInterval(MultiEdgeCounter &current,
			MultiEdgeCounter &previous, NodeFilter &prev_nodes,
			int interval_counter);

private:
	string _edgeOutputFolderName;
	bool _enable_multiple_edges;
	ListDigraph &_graph;
	ListDigraph::NodeMap<NodeId> &_id;
	log4cpp::Category* _logger;
	bool _console_logging_enabled;
};

#endif
