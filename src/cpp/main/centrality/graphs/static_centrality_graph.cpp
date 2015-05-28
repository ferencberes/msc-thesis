#include "static_centrality_graph.h"

#include <lemon/adaptors.h>
#include <lemon/bfs.h>

using namespace lemon;

void StaticCentralityGraph::initializeMaps() {
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		int out_num = 0;
		for (ListDigraph::OutArcIt out_edge(_graph, v); out_edge != INVALID;
				++out_edge) {
			out_num++;
		}
		_num_of_out_edges[v] = out_num;

		int in_num = 0;
		for (ListDigraph::InArcIt in_edge(_graph, v); in_edge != INVALID;
				++in_edge) {
			in_num++;
		}
		_num_of_in_edges[v] = in_num;

	}
}

void StaticCentralityGraph::printNeighbours() {
	std::cout << std::endl;
	std::cout << "Vertices with neighbour data:" << std::endl;
	TopKSelector in_selector(_topKNumber);
	TopKSelector out_selector(_topKNumber);
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		in_selector.push(IdValuePair(_id[v], _num_of_in_edges[v]));
		out_selector.push(IdValuePair(_id[v], _num_of_out_edges[v]));
	}
	in_selector.sort();
	out_selector.sort();

	std::cout << "TopK indegree:" << std::endl;
	for (std::vector<IdValuePair>::size_type i = 0; i < in_selector.vec.size();
			i++) {
		(in_selector.vec[i]).str();
	}

	std::cout << "TopK outdegree:" << std::endl;
	for (std::vector<IdValuePair>::size_type i = 0; i < out_selector.vec.size();
			i++) {
		(out_selector.vec[i]).str();
	}
}

void StaticCentralityGraph::printCentralityMeasureMap(std::string name,
		CentralityMeasureMap& map) {
	std::cout << std::endl;
	std::cout << name << " results:" << std::endl;
	TopKSelector selector(_topKNumber);
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		selector.push(IdValuePair(_id[v], map[v]));
	}
	selector.sort();
	for (std::vector<IdValuePair>::size_type i = 0; i < selector.vec.size();
			i++) {
		(selector.vec[i]).str();
	}
}

void StaticCentralityGraph::printSalsaMeasureMap(SalsaMeasureMap& map) {
	std::cout << std::endl;
	std::cout << "Salsa-score results:" << std::endl;
	TopKSelector auth_selector(_topKNumber);
	TopKSelector hub_selector(_topKNumber);
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		auth_selector.push(IdValuePair(_id[v], map[v].authority));
		hub_selector.push(IdValuePair(_id[v], map[v].hub));
	}
	auth_selector.sort();
	hub_selector.sort();

	std::cout << "TopK authority score:" << std::endl;
	for (std::vector<IdValuePair>::size_type i = 0;
			i < auth_selector.vec.size(); i++) {
		(auth_selector.vec[i]).str();
	}

	std::cout << "TopK hub score:" << std::endl;
	for (std::vector<IdValuePair>::size_type i = 0; i < hub_selector.vec.size();
			i++) {
		(hub_selector.vec[i]).str();
	}
}

void StaticCentralityGraph::computeLoopEdgeRatio() {
	clock_t begin = clock();
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		float num_of_loops = 0.0;
		float num_of_normal = 0.0;
		for (ListDigraph::OutArcIt out_edge(_graph, v); out_edge != INVALID;
				++out_edge) {
			if (_id[_graph.target(out_edge)] == _id[v]) {
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
	printCentralityMeasureMap("Loop-edge ratio", _finalRatio);
	std::cout << "Elapsed time for loop-edge ratio computing: " << elapsed_secs
			<< " sec." << std::endl;
}

void StaticCentralityGraph::computeBetaMeasure() {
	clock_t begin = clock();
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		float summed_beta_for_vertex = 0.0;
		for (ListDigraph::InArcIt in_edge(_graph, v); in_edge != INVALID;
				++in_edge) {
			summed_beta_for_vertex += 1.0
					/ _num_of_out_edges[_graph.source(in_edge)];
		}
		_finalBeta[v] = summed_beta_for_vertex;
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	printCentralityMeasureMap("Beta-measure", _finalBeta);
	std::cout << "Elapsed time for Beta-measure computing: " << elapsed_secs
			<< " sec." << std::endl;
}

void StaticCentralityGraph::computePageRank(float dampening_factor,
		int number_of_iterations) {
	clock_t begin = clock();
	float value_from_teleport = (1 - dampening_factor) / numOfVertices();
	for (int iter = 0; iter < number_of_iterations; iter++) {
		for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
			float incoming_pagerank = 0.0;
			for (ListDigraph::InArcIt in_edge(_graph, v); in_edge != INVALID;
					++in_edge) {
				incoming_pagerank += _finalPageRank[_graph.source(in_edge)]
						/ _num_of_out_edges[_graph.source(in_edge)];
			}
			_finalPageRank[v] = value_from_teleport
					+ dampening_factor * incoming_pagerank;
		}
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	printCentralityMeasureMap("PageRank", _finalPageRank);
	std::cout << "Elapsed time for Pagerank computing: " << elapsed_secs
			<< " sec." << std::endl;
}

void StaticCentralityGraph::computeSalsa(int number_of_iterations) {
	clock_t begin = clock();
	for (int iter = 0; iter < 2 * number_of_iterations; iter++) {
		if (iter % 2 == 0) {
			for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
				float incoming_hub = 0.0;
				for (ListDigraph::OutArcIt out_edge(_graph, v);
						out_edge != INVALID; ++out_edge) {
					incoming_hub +=
							_finalSalsa[_graph.target(out_edge)].authority
									/ _num_of_in_edges[_graph.target(out_edge)];
				}
				_finalSalsa[v].setHubValue(incoming_hub);
			}
		} else {
			for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
				float incoming_auth = 0.0;
				for (ListDigraph::InArcIt in_edge(_graph, v);
						in_edge != INVALID; ++in_edge) {
					incoming_auth += _finalSalsa[_graph.source(in_edge)].hub
							/ _num_of_out_edges[_graph.source(in_edge)];
				}
				_finalSalsa[v].setAuthorityValue(incoming_auth);
			}
		}
	}
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	printSalsaMeasureMap(_finalSalsa);
	std::cout << "Elapsed time for Salsa computing: " << elapsed_secs << " sec."
			<< std::endl;
}

void StaticCentralityGraph::computeHarmonic() {
	typedef ReverseDigraph<ListDigraph> RevListDigraph;
	ListDigraph *g_p;
	g_p = &_graph;
	RevListDigraph reverse_graph(*g_p);
	std::cout
			<< "This is just a dummy implementation. Do not run this on huge graphs!!!"
			<< std::endl;

	clock_t begin = clock();
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		Bfs<RevListDigraph> bfs(reverse_graph);
		bfs.run(v);
		float score_to_vertex = 0.0;

		for (ListDigraph::NodeIt t(_graph); t != INVALID; ++t) {
			if (bfs.reached(t) && v != t) {
				score_to_vertex += 1 / bfs.dist(t);
			}
		}
		_finalHarmonicScore[v] = score_to_vertex;
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	printCentralityMeasureMap("Harmonic-score", _finalHarmonicScore);
	std::cout << "Elapsed time for Harmonic-score computing: " << elapsed_secs
			<< " sec." << std::endl;

}

int StaticCentralityGraph::getNumOfInEdges(NodeId id) {
	int output = -1;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _num_of_in_edges[v];
			break;
		}
	}
	return output;
}

int StaticCentralityGraph::getNumOfOutEdges(NodeId id) {
	int output = -1;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _num_of_out_edges[v];
			break;
		}
	}
	return output;
}

float StaticCentralityGraph::getBetaScore(NodeId id) {
	float output = -1.0;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalBeta[v];
			break;
		}
	}
	return output;
}

float StaticCentralityGraph::getHarmonicScore(NodeId id) {
	float output = -1.0;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalHarmonicScore[v];
			break;
		}
	}
	return output;
}

float StaticCentralityGraph::getPageRank(NodeId id) {
	float output = -1.0;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalPageRank[v];
			break;
		}
	}
	return output;
}

float StaticCentralityGraph::getLoopRatio(NodeId id) {
	float output = -1.0;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalRatio[v];
			break;
		}
	}
	return output;
}

float StaticCentralityGraph::getSalsaAuthority(NodeId id) {
	float output = -1.0;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalSalsa[v].authority;
			break;
		}
	}
	return output;
}

float StaticCentralityGraph::getSalsaHub(NodeId id) {
	float output = -1.0;
	for (ListDigraph::NodeIt v(_graph); v != INVALID; ++v) {
		if (_id[v] == id) {
			output = _finalSalsa[v].hub;
			break;
		}
	}
	return output;
}
