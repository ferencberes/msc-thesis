#ifndef TEMPORAL_LIST_DIGRAPH_FOR_CENTRALITY_H
#define TEMPORAL_LIST_DIGRAPH_FOR_CENTRALITY_H

#include "../utils/temporal_centrality_utils.h"

using namespace lemon;

class TemporalCentralityGraph {
public:
	TemporalCentralityGraph(TemporalSubGraph &g,
			ListDigraph::NodeMap<NodeId> &vertexIds,
			EdgeWeightMap &edge_weights, int topKNumber, bool enable_logging) :
			_subGraph(g), _topKNumber(topKNumber), _id(vertexIds), _weights(
					edge_weights), _num_of_in_edges(_subGraph, 0), _num_of_out_edges(
					_subGraph, 0), _finalRatio(_subGraph, 0.0), _finalBeta(_subGraph,
					0.0), _finalPageRank(_subGraph, 0.0), _finalSalsa(_subGraph, salsa_data()), _finalHarmonicScore(
					_subGraph, 0.0), _console_logging_enabled(enable_logging) {
		log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
		layout->setConversionPattern("%d [%p] %m%n");
		log4cpp::Appender *consol_appender = new log4cpp::OstreamAppender(
				"console", &std::cout);
		consol_appender->setLayout(layout);
		_logger = &log4cpp::Category::getInstance(
				std::string("TemporalListDigraphForCentrality"));
		_logger->addAppender(consol_appender);
		setLogLevel(_console_logging_enabled);
		_logger->info(
				"TemporalListDigraphForCentrality object was initialized.");
		updateDegreeMaps();
	}

	const TemporalSubGraph& getGraph() const {
		return _subGraph;
	}

	int numOfVertices() {
		return countNodes(_subGraph);
	}

	int numOfEdges() {
		return countArcs(_subGraph);
	}

	int getTopK() const {
		return _topKNumber;
	}

	void setLogLevel(bool enabled);
	void updateDegreeMaps();
	void sortNeighbours();
	void sortCentralityMeasureMap(TemporalCentralityMeasureMap& map);
	void sortSalsaMeasureMaps(const TemporalSalsaMeasureMap& map);
	TopKSelector getTopDegree(const TemporalNeighbourMap& map);
	TopKSelector getTopCentralityMeasureMap(
			const TemporalCentralityMeasureMap& map);
	TopKSelector getTopSalsaMeasureMap(const TemporalSalsaMeasureMap& map,
			bool getAuth);
	void computeLoopEdgeRatio();
	void computeBetaMeasure();
	void computeHarmonic();
	void computePageRank(float dampening_factor, int number_of_iterations);
	void computeSalsa(int number_of_iterations);
	int getNumOfInEdges(NodeId id);
	int getNumOfOutEdges(NodeId id);
	float getBetaScore(NodeId id);
	float getHarmonicScore(NodeId id);
	float getPageRank(NodeId id);
	float getLoopRatio(NodeId id);
	float getSalsaAuthority(NodeId id);
	float getSalsaHub(NodeId id);

	const TemporalCentralityMeasureMap& getFinalBeta() const {
		return _finalBeta;
	}

	const TemporalCentralityMeasureMap& getFinalPageRank() const {
		return _finalPageRank;
	}

	const TemporalCentralityMeasureMap& getFinalRatio() const {
		return _finalRatio;
	}

	const TemporalCentralityMeasureMap& getFinalHarmonicScore() const {
		return _finalHarmonicScore;
	}

	const TemporalSalsaMeasureMap& getFinalSalsa() const {
		return _finalSalsa;
	}

	const TemporalNeighbourMap& getFinalOutEdges() const {
		return _num_of_out_edges;
	}

	const TemporalNeighbourMap& getFinalInEdges() const {
		return _num_of_in_edges;
	}

private:
	TemporalSubGraph &_subGraph;
	int _topKNumber;

	// subgraph information
	IdentifierMap &_id;
	EdgeWeightMap &_weights;
	TemporalNeighbourMap _num_of_in_edges;
	TemporalNeighbourMap _num_of_out_edges;

	// centrality measures
	TemporalCentralityMeasureMap _finalRatio;
	TemporalCentralityMeasureMap _finalBeta;
	TemporalCentralityMeasureMap _finalPageRank;
	TemporalSalsaMeasureMap _finalSalsa;
	TemporalCentralityMeasureMap _finalHarmonicScore;

	// console logging
	log4cpp::Category* _logger;
	bool _console_logging_enabled;

};

#endif
