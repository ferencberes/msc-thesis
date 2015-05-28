#ifndef TEMPORAL_CENTRALITY_UTILS_H
#define TEMPORAL_CENTRALITY_UTILS_H

#include <strings.h>
#include <queue>
#include <set>

// all log4cpp include is done here
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include <log4cpp/PatternLayout.hh>
#include "log4cpp/Priority.hh"

#include <lemon/adaptors.h>
#include "centrality_utils.h"

using namespace lemon;

typedef SubDigraph<ListDigraph> TemporalSubGraph;
typedef ListDigraph::NodeMap<bool> NodeFilter;
typedef ListDigraph::ArcMap<bool> EdgeFilter;
typedef ListDigraph::ArcMap<float> EdgeWeightMap;
typedef ReverseDigraph<TemporalSubGraph> RevTemporalSubGraph;

typedef std::priority_queue<long, std::vector<long>, std::greater<long> > mention_time_list;
typedef std::map<ListDigraph::Node, mention_time_list> counter_map;
typedef std::map<ListDigraph::Node, mention_time_list>::iterator counter_map_iterator;
typedef ListDigraph::NodeMap<counter_map> MultiEdgeCounter;

typedef TemporalSubGraph::NodeMap<int> TemporalNeighbourMap;
typedef TemporalSubGraph::NodeMap<float> TemporalCentralityMeasureMap;
typedef TemporalSubGraph::NodeMap<salsa_data> TemporalSalsaMeasureMap;

bool string2bool(const std::string & v);

struct recsys_record {
	long mention_time;
	NodeId source;
	NodeId target;
	double value;
	int eval;

	recsys_record(long m_time, NodeId src, NodeId trg, double val, int ev) {
		mention_time = m_time;
		source = src;
		target = trg;
		value = val;
		eval = ev;
	}
	std::string print();
	std::string printHomophily();
};

typedef std::map<ListDigraph::Node, long> first_mention_map;
typedef std::map<ListDigraph::Node, long>::iterator first_mention_map_iter;
typedef ListDigraph::NodeMap<first_mention_map> GlobalEdgeCounter;
typedef std::priority_queue<recsys_record> GlobalEdgeTimeQueue;
// operator for ordered_edges_list to order the minimum on top
bool operator<(const recsys_record& a, const recsys_record& b);
typedef std::priority_queue<recsys_record> ordered_edges_list;

void printNodeMap(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
		NodeFilter &map);

void printEdgeMap(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
		EdgeFilter &map);

void printEdgeCounterMap(ListDigraph &graph, ListDigraph::NodeMap<NodeId> &id,
		MultiEdgeCounter &counter_map);

#endif
