#ifndef LIST_DIGRAPH_FOR_CENTRALITY_H
#define LIST_DIGRAPH_FOR_CENTRALITY_H

#include "../utils/centrality_utils.h"

using namespace lemon;

class StaticCentralityGraph {
public:
	StaticCentralityGraph(ListDigraph &g, ListDigraph::NodeMap<NodeId> &vertexIds,
			int topKNumber) :
			_graph(g), _id(vertexIds), _num_of_in_edges(_graph), _num_of_out_edges(
					_graph), _finalRatio(_graph), _finalBeta(_graph, 0), _finalPageRank(
					_graph, 1.0 / numOfVertices()), _finalSalsa(_graph,
					salsa_data()), _finalHarmonicScore(_graph) {
		_topKNumber = topKNumber;
		initializeMaps();
	}

	const ListDigraph& getGraph() const {
		return _graph;
	}

	int numOfVertices() {
		return countNodes(_graph);
	}

	int numOfEdges() {
		return countArcs(_graph);
	}

	void initializeMaps();
	void printNeighbours();
	void printCentralityMeasureMap(std::string name, CentralityMeasureMap& map);
	void printSalsaMeasureMap(SalsaMeasureMap& map);
	void computeLoopEdgeRatio();
	void computeBetaMeasure();
	void computePageRank(float dampening_factor, int number_of_iterations);
	void computeSalsa(int number_of_iterations);
	void computeHarmonic();
	int getNumOfInEdges(NodeId id);
	int getNumOfOutEdges(NodeId id);
	float getBetaScore(NodeId id);
	float getHarmonicScore(NodeId id);
	float getPageRank(NodeId id);
	float getLoopRatio(NodeId id);
	float getSalsaAuthority(NodeId id);
	float getSalsaHub(NodeId id);

private:
	ListDigraph &_graph;
	int _topKNumber;

	//graph information
	IdentifierMap &_id;
	NeighbourMap _num_of_in_edges;
	NeighbourMap _num_of_out_edges;

	//centrality measures
	CentralityMeasureMap _finalRatio;
	CentralityMeasureMap _finalBeta;
	CentralityMeasureMap _finalPageRank;
	SalsaMeasureMap _finalSalsa;
	CentralityMeasureMap _finalHarmonicScore;

};

#endif
