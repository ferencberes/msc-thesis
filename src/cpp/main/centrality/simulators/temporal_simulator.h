#ifndef TEMPORAL_SIMULATOR_H
#define TEMPORAL_SIMULATOR_H

#include "../graphs/temporal_centrality_graph.h"
#include "../utils/temporal_graph_stat_counter.h"
#include "../writers/centrality_json_writer.h"
#include "../writers/recsys_edge_writer.h"

#include <limits.h>

using namespace lemon;

const int NUM_OF_ITERATION = 10;

class TemporalSimulator {
public:
	TemporalSimulator(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
			ListDigraph::ArcMap<long> &mention_time,
			EdgeWeightMap &edge_weights, char* output_folder) :
			_graph(graph), _id(id), _mention_time(mention_time), _edge_weights(edge_weights),
			_output_folder(output_folder), _combine_factor(-1.0), _centrality_prev_interval_count(0),
			_graph_stat_prev_interval_count(-1),_enable_multi_edges(false),
			_runDegree(false), _runBeta(false), _runPageRank(false),
			_runSalsa(false), _runLoop(false), _runHarmonic(false),
			_startTime(0), _minTime(INT_MAX), _stopTime(0), _delta(0),
			_numOfSteps(0), _topKNumber(0) {

		_valid_config = false;
		if (output_folder == std::string("").c_str()) {
			_writers_enabled = false;
		} else {
			_writers_enabled = true;
		}

		log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
		layout->setConversionPattern("%d [%p] %m%n");
		log4cpp::Appender *consol_appender = new log4cpp::OstreamAppender(
				"console", &std::cout);
		consol_appender->setLayout(layout);

		_logger = &log4cpp::Category::getInstance(
				std::string("TemporalSimulator"));
		_logger->addAppender(consol_appender);
		_console_logging_enabled = true;
		setLogLevel(_console_logging_enabled);
	}

	void setLogLevel(bool enabled);
	void getGraphInfo();
	void getTimeInfo();
	void getTimeBounds();
	virtual void setConfigForSimulation(int argc, char* argv[]);
	void setConfigVariables(long startTime, long delta,
			int centrality_lookback_count, float combine_factor,
			int graph_stat_lookback_count, bool enable_multi_edges,
			int numOfSteps, int topKNumber, bool runDegree, bool runBeta,
			bool runPageRank, bool runLoop, bool runSalsa, bool runHarmonic);
	void simulate();
	void filter_edges_for_global_time_queue(
			GlobalEdgeCounter &global_edge_counter,
			GlobalEdgeTimeQueue &global_edge_time_queue);
	virtual void filter_current_interval_graph(int interval_count,
			long actualTime, NodeFilter &current_nodes,
			EdgeFilter &current_edges, MultiEdgeCounter &current_edge_counter,
			GlobalEdgeCounter &global_edge_counter);
	void filter_prev_interval_graph(long actualTime, NodeFilter &prev_nodes,
			MultiEdgeCounter &prev_edge_counter);
	void computeCentralityMeasures(int iteration_counter,
			TemporalCentralityGraph &cg);
	virtual void showUsage();

	void printInterfaceError();

	const vector<long>& getNewEdgeChanges() const {
		return _new_edge_changes;
	}

	const vector<long>& getNewHomEdgeChanges() const {
		return _new_hom_edge_changes;
	}

	const vector<long>& getNewNodeChanges() const {
		return _new_node_changes;
	}

	const vector<long>& getPreviousEdgeChanges() const {
		return _previous_edge_changes;
	}

	const vector<long>& getPreviousNodeChanges() const {
		return _previous_node_changes;
	}

	const vector<long>& getDeletedEdgeChanges() const {
		return _deleted_edge_changes;
	}

	const vector<long>& getDeletedNodeChanges() const {
		return _deleted_node_changes;
	}

	const vector<long>& getCurrentEdgeChanges() const {
		return _current_edge_changes;
	}

	const vector<long>& getCurrentNodeChanges() const {
		return _current_node_changes;
	}

	float getCombineFactor() const {
		return _combine_factor;
	}

protected:
	log4cpp::Category* _logger;
	bool _console_logging_enabled;

	ListDigraph &_graph;
	ListDigraph::NodeMap<NodeId> &_id;
	ListDigraph::ArcMap<long> &_mention_time;
	EdgeWeightMap &_edge_weights;

	char* _output_folder;
	bool _writers_enabled;

	float _combine_factor;
	int _centrality_prev_interval_count;
	int _graph_stat_prev_interval_count;
	bool _enable_multi_edges;
	bool _valid_config;

	vector<string> _measureSet;
	bool _runDegree;
	bool _runBeta;
	bool _runPageRank;
	bool _runSalsa;
	bool _runLoop;
	bool _runHarmonic;

	long _startTime;
	long _minTime;
	long _stopTime;
	long _delta;
	int _numOfSteps;
	int _topKNumber;

	vector<long> _current_node_changes;
	vector<long> _current_edge_changes;
	vector<long> _new_edge_changes;
	vector<long> _new_hom_edge_changes;
	vector<long> _new_node_changes;
	vector<long> _previous_node_changes;
	vector<long> _previous_edge_changes;
	vector<long> _deleted_node_changes;
	vector<long> _deleted_edge_changes;
}
;

#endif
