#include "temporal_centrality_graph.h"

#include <lemon/bfs.h>
#include <string.h> // memcpy
#include <ctime>

using namespace lemon;

void TemporalCentralityGraph::setLogLevel(bool enabled) {
	enabled ?
			_logger->setPriority(log4cpp::Priority::INFO) :
			_logger->setPriority(log4cpp::Priority::ERROR);
	_console_logging_enabled = enabled;
}

void TemporalCentralityGraph::updateDegreeMaps() {
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		int out_num = 0;
		for (TemporalSubGraph::OutArcIt out_edge(_subGraph, v);
				out_edge != INVALID; ++out_edge) {
			out_num++;
		}
		_num_of_out_edges[v] = out_num;

		int in_num = 0;
		for (TemporalSubGraph::InArcIt in_edge(_subGraph, v);
				in_edge != INVALID; ++in_edge) {
			in_num++;
		}
		_num_of_in_edges[v] = in_num;
	}
	_logger->info("Degree maps updated.");
}

void TemporalCentralityGraph::sortNeighbours() {
	TopKSelector in_selector(_topKNumber);
	TopKSelector out_selector(_topKNumber);
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		in_selector.push(IdValuePair(_id[v], _num_of_in_edges[v]));
		out_selector.push(IdValuePair(_id[v], _num_of_out_edges[v]));
	}
	in_selector.sort();
	out_selector.sort();
}

void TemporalCentralityGraph::sortCentralityMeasureMap(
		TemporalCentralityMeasureMap& map) {
	TopKSelector selector(_topKNumber);
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		selector.push(IdValuePair(_id[v], map[v]));
	}
	selector.sort();
}

void TemporalCentralityGraph::sortSalsaMeasureMaps(
		const TemporalSalsaMeasureMap& map) {
	TopKSelector auth_selector(_topKNumber);
	TopKSelector hub_selector(_topKNumber);
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		auth_selector.push(IdValuePair(_id[v], map[v].authority));
		hub_selector.push(IdValuePair(_id[v], map[v].hub));
	}
	auth_selector.sort();
	hub_selector.sort();
}

TopKSelector TemporalCentralityGraph::getTopDegree(
		const TemporalNeighbourMap& map) {
	TopKSelector degree_selector(_topKNumber);
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		degree_selector.push(IdValuePair(_id[v], map[v]));
	}
	degree_selector.sort();
	return degree_selector;
}

TopKSelector TemporalCentralityGraph::getTopCentralityMeasureMap(
		const TemporalCentralityMeasureMap& map) {
	TopKSelector selector(_topKNumber);
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		selector.push(IdValuePair(_id[v], map[v]));
	}
	selector.sort();
	return selector;
}

TopKSelector TemporalCentralityGraph::getTopSalsaMeasureMap(
		const TemporalSalsaMeasureMap& map, bool getAuth) {
	TopKSelector salsa_selector(_topKNumber);
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (getAuth) {
			salsa_selector.push(IdValuePair(_id[v], map[v].authority));
		} else {
			salsa_selector.push(IdValuePair(_id[v], map[v].hub));
		}
	}
	salsa_selector.sort();
	return salsa_selector;
}

void TemporalCentralityGraph::computeLoopEdgeRatio() {
	clock_t begin = clock();
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		float num_of_loops = 0.0;
		float num_of_normal = 0.0;
		for (TemporalSubGraph::OutArcIt out_edge(_subGraph, v);
				out_edge != INVALID; ++out_edge) {
			if (_id[_subGraph.target(out_edge)] == _id[v]) {
				num_of_loops++;
			} else {
				num_of_normal++;
			}
			if (num_of_normal > 0) {
				_finalRatio[v] = num_of_loops / num_of_normal;
			} else {
				_finalRatio[v] = num_of_loops; //TODO: vagy ilyenkor állítsuk 0-ra a ratio-t?
			}
		}
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	sortCentralityMeasureMap(_finalRatio);
	_logger->infoStream() << "Elapsed time for loop-edge ratio computing: "
			<< elapsed_secs << " sec.";
}

void TemporalCentralityGraph::computeBetaMeasure() {
	clock_t begin = clock();
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		float summed_beta_for_vertex = 0.0;
		for (TemporalSubGraph::InArcIt in_edge(_subGraph, v);
				in_edge != INVALID; ++in_edge) {
			summed_beta_for_vertex += _weights[in_edge]
					/ _num_of_out_edges[_subGraph.source(in_edge)];
		}
		_finalBeta[v] = summed_beta_for_vertex;
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	sortCentralityMeasureMap(_finalBeta);
	_logger->infoStream() << "Elapsed time for Beta-measure computing: "
			<< elapsed_secs << " sec.";
}

void TemporalCentralityGraph::computeHarmonic() {

	TemporalSubGraph *g_p;
	g_p = &_subGraph;
	RevTemporalSubGraph reverse_graph(*g_p);

	_logger->warnStream()
			<< "Do not run harmonic centrality computing on big graphs!!!";

	clock_t begin = clock();
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		Bfs<RevTemporalSubGraph> bfs(reverse_graph);
		bfs.run(v);
		float score_to_vertex = 0.0;

		for (TemporalSubGraph::NodeIt t(_subGraph); t != INVALID; ++t) {
			if (bfs.reached(t) && v != t) {
				score_to_vertex += 1 / bfs.dist(t);
			}
		}
		_finalHarmonicScore[v] = score_to_vertex;
	}

	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	sortCentralityMeasureMap(_finalHarmonicScore);
	_logger->infoStream() << "Elapsed time for Harmonic-score computing: "
			<< elapsed_secs << " sec.";

}

void TemporalCentralityGraph::computePageRank(float dampening_factor,
		int number_of_iterations) {
	clock_t begin = clock();

	// TODO: from some reason the declaration of finalPageRank did not work in the constructor!!! But here it works fine.
	float initial_value = 1 / numOfVertices();
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		_finalPageRank[v] = initial_value;
	}

	float value_from_teleport = (1 - dampening_factor) / numOfVertices();
	for (int iter = 0; iter < number_of_iterations; iter++) {
		for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
			float incoming_pagerank = 0.0;
			for (TemporalSubGraph::InArcIt in_edge(_subGraph, v);
					in_edge != INVALID; ++in_edge) {
				incoming_pagerank += _weights[in_edge]
						* _finalPageRank[_subGraph.source(in_edge)]
						/ _num_of_out_edges[_subGraph.source(in_edge)];
			}
			_finalPageRank[v] = value_from_teleport
					+ dampening_factor * incoming_pagerank;
		}
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	sortCentralityMeasureMap(_finalPageRank);
	_logger->infoStream() << "Elapsed time for Pagerank computing: "
			<< elapsed_secs << " sec.";
}

void TemporalCentralityGraph::computeSalsa(int number_of_iterations) {
	clock_t begin = clock();
	for (int iter = 0; iter < 2 * number_of_iterations; iter++) {
		if (iter % 2 == 0) {
			for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
				float incoming_hub = 0.0;
				for (TemporalSubGraph::OutArcIt out_edge(_subGraph, v);
						out_edge != INVALID; ++out_edge) {
					incoming_hub += _weights[out_edge]
							* _finalSalsa[_subGraph.target(out_edge)].authority
							/ _num_of_in_edges[_subGraph.target(out_edge)];
				}
				_finalSalsa[v].setHubValue(incoming_hub);
			}
		} else {
			for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
				float incoming_auth = 0.0;
				for (TemporalSubGraph::InArcIt in_edge(_subGraph, v);
						in_edge != INVALID; ++in_edge) {
					incoming_auth += _weights[in_edge]
							* _finalSalsa[_subGraph.source(in_edge)].hub
							/ _num_of_out_edges[_subGraph.source(in_edge)];
				}
				_finalSalsa[v].setAuthorityValue(incoming_auth);
			}
		}
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	sortSalsaMeasureMaps(_finalSalsa);
	_logger->infoStream() << "Elapsed time for Salsa computing: "
			<< elapsed_secs << " sec.";
}

int TemporalCentralityGraph::getNumOfInEdges(NodeId id) {
	int output = -1;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _num_of_in_edges[v];
			break;
		}
	}
	return output;
}

int TemporalCentralityGraph::getNumOfOutEdges(NodeId id) {
	int output = -1;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _num_of_out_edges[v];
			break;
		}
	}
	return output;
}

float TemporalCentralityGraph::getBetaScore(NodeId id) {
	float output = -1.0;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalBeta[v];
			break;
		}
	}
	return output;
}

float TemporalCentralityGraph::getHarmonicScore(NodeId id) {
	float output = -1.0;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalHarmonicScore[v];
			break;
		}
	}
	return output;
}

float TemporalCentralityGraph::getPageRank(NodeId id) {
	float output = -1.0;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalPageRank[v];
			break;
		}
	}
	return output;
}

float TemporalCentralityGraph::getLoopRatio(NodeId id) {
	float output = -1.0;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalRatio[v];
			break;
		}
	}
	return output;
}

float TemporalCentralityGraph::getSalsaAuthority(NodeId id) {
	float output = -1.0;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalSalsa[v].authority;
			break;
		}
	}
	return output;
}

float TemporalCentralityGraph::getSalsaHub(NodeId id) {
	float output = -1.0;
	for (TemporalSubGraph::NodeIt v(_subGraph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalSalsa[v].hub;
			break;
		}
	}
	return output;
}

