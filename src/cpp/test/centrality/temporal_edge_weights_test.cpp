#include <gtest/gtest.h>

#include "../../main/centrality/simulators/basic_temporal_simulator.h"
#include "../../main/centrality/simulators/temporal_simulator_for_predictions.h"

namespace {

using namespace lemon;
using namespace std;

const float DELTA = 0.00001;

void initializeGraph(ListDigraph &g, ListDigraph::NodeMap<NodeId> &id,
		ListDigraph::ArcMap<long> &mention_time, EdgeWeightMap &edge_weights) {
	ListDigraph::Node v0 = g.addNode();
	id[v0] = 0;
	ListDigraph::Node v1 = g.addNode();
	id[v1] = 1;
	ListDigraph::Node v2 = g.addNode();
	id[v2] = 2;
	ListDigraph::Node v3 = g.addNode();
	id[v3] = 3;
	ListDigraph::Node v4 = g.addNode();
	id[v4] = 4;
	ListDigraph::Node v5 = g.addNode();
	id[v5] = 5;
	ListDigraph::Node v6 = g.addNode();
	id[v6] = 6;
	ListDigraph::Node v7 = g.addNode();
	id[v7] = 7;
	ListDigraph::Node v8 = g.addNode();
	id[v8] = 8;
	ListDigraph::Node v9 = g.addNode();
	id[v9] = 9;
	ListDigraph::Node v10 = g.addNode();
	id[v10] = 10;

	// [0,1) interval
	ListDigraph::Arc a0 = g.addArc(v0, v1);
	mention_time[a0] = 0;
	ListDigraph::Arc a1 = g.addArc(v0, v2);
	mention_time[a1] = 0;
	ListDigraph::Arc a15 = g.addArc(v1, v2);
	mention_time[a15] = 0;

	// [1,2) interval
	ListDigraph::Arc a2 = g.addArc(v1, v3);
	mention_time[a2] = 1;
	ListDigraph::Arc a3 = g.addArc(v1, v4);
	mention_time[a3] = 1;
	ListDigraph::Arc a4 = g.addArc(v2, v4);
	mention_time[a4] = 1;

	// [2,3) interval
	ListDigraph::Arc a5 = g.addArc(v0, v3);
	mention_time[a5] = 2;
	ListDigraph::Arc a6 = g.addArc(v0, v5);
	mention_time[a6] = 2;
	ListDigraph::Arc a7 = g.addArc(v0, v6);
	mention_time[a7] = 2;
	edge_weights[a7] = 1.0;
	ListDigraph::Arc a8 = g.addArc(v0, v1);
	mention_time[a8] = 2;
	ListDigraph::Arc a9 = g.addArc(v0, v1);
	mention_time[a9] = 2;
	ListDigraph::Arc a10 = g.addArc(v1, v4);
	mention_time[a10] = 2;
	ListDigraph::Arc a11 = g.addArc(v2, v0);
	mention_time[a11] = 2;
	ListDigraph::Arc a12 = g.addArc(v2, v1);
	mention_time[a12] = 2;
	ListDigraph::Arc a13 = g.addArc(v0, v5);
	mention_time[a13] = 2;
	ListDigraph::Arc a14 = g.addArc(v0, v7);
	mention_time[a14] = 2;

	// [3,4) interval
	ListDigraph::Arc a16 = g.addArc(v0, v5);
	mention_time[a16] = 3;
	ListDigraph::Arc a17 = g.addArc(v0, v6);
	mention_time[a17] = 3;
	ListDigraph::Arc a18 = g.addArc(v8, v9);
	mention_time[a18] = 3;
	ListDigraph::Arc a19 = g.addArc(v9, v10);
	mention_time[a19] = 3;
	ListDigraph::Arc a20 = g.addArc(v0, v2);
	mention_time[a20] = 3;
}

class BasicSimulatorWithWeights: public ::testing::Test {
protected:
	ListDigraph g;
	ListDigraph::NodeMap<NodeId> id;
	ListDigraph::ArcMap<long> mention_time;
	EdgeWeightMap edge_weights;
	BasicTemporalSimulator simulator;

	BasicSimulatorWithWeights() :
			id(g), mention_time(g), edge_weights(g), simulator(g, id,
					mention_time, edge_weights, "") {
		initializeGraph(g, id, mention_time, edge_weights);
		// turn off console logging
		simulator.setLogLevel(false);
	}

	virtual void SetUp() {
		// set edge weight uniformly 1.0 before each test
		for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
			edge_weights[a] = 1.0;
		}
	}
};

class SimulatorForPredictionsWithWeights: public ::testing::Test {
protected:
	ListDigraph g;
	ListDigraph::NodeMap<NodeId> id;
	ListDigraph::ArcMap<long> mention_time;
	EdgeWeightMap edge_weights;
	TemporalSimulatorForPredictions simulator;

	SimulatorForPredictionsWithWeights() :
			id(g), mention_time(g), edge_weights(g), simulator(g, id,
					mention_time, edge_weights, "", "link_prediction_test", 4) {
		initializeGraph(g, id, mention_time, edge_weights);
		// turn off console logging
		simulator.setLogLevel(false);
	}

	virtual void SetUp() {
		// set edge weight uniformly 1.0 before each test
		for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
			edge_weights[a] = 1.0;
		}
	}
};

}

TEST_F(BasicSimulatorWithWeights, one_interval_without_combine) {
	simulator.setConfigVariables(0, 1, 0, -1.0, -1, true, 1, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	ASSERT_NEAR(-1.0, simulator.getCombineFactor(), DELTA);
	// the weights of the edges must not change for combine_factor=-1.0
	for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
		EXPECT_NEAR(1.0, edge_weights[a], DELTA);
	}
}

TEST_F(BasicSimulatorWithWeights, one_interval_with_combine) {
	simulator.setConfigVariables(0, 1, 0, 0.2, -1, true, 1, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	ASSERT_NEAR(0.2, simulator.getCombineFactor(), DELTA);
	// the weights of edges are multiplied with 0.8 for previous interval
	for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
		if (mention_time[a] < 1) {
			EXPECT_NEAR(0.8, edge_weights[a], DELTA);
		} else {
			EXPECT_NEAR(1.0, edge_weights[a], DELTA);
		}
	}
}

TEST_F(BasicSimulatorWithWeights, three_interval_without_combine) {
	simulator.setConfigVariables(0, 1, 0, -1.0, -1, true, 3, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	ASSERT_NEAR(-1.0, simulator.getCombineFactor(), DELTA);
	// the weights of the edges must not change for combine_factor=-1.0
	for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
		EXPECT_NEAR(1.0, edge_weights[a], DELTA);
	}
}

TEST_F(BasicSimulatorWithWeights, three_interval_with_combine_and_no_lookback) {
	simulator.setConfigVariables(0, 1, 0, 0.3, -1, true, 4, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	ASSERT_NEAR(0.3, simulator.getCombineFactor(), DELTA);
	// the weights of edges are multiplied with 0.7 for current
	for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
		if (mention_time[a] > 2) {
			// current edges
			EXPECT_NEAR(0.7, edge_weights[a], DELTA);
		} else if (mention_time[a] > 1) {
			// previous edges
			EXPECT_NEAR(1.0, edge_weights[a], DELTA);
		} else {
			// others
			EXPECT_NEAR(1.0, edge_weights[a], DELTA);
		}
	}
}

TEST_F(BasicSimulatorWithWeights, three_interval_with_combine_and_lookback) {
	simulator.setConfigVariables(0, 1, 1, 0.3, -1, true, 4, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	ASSERT_NEAR(0.3, simulator.getCombineFactor(), DELTA);
	// the weights of edges are multiplied with 0.7 for current and 0.3 with previous
	for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
		if (mention_time[a] > 2) {
			// current edges
			EXPECT_NEAR(0.7, edge_weights[a], DELTA);
		} else if (mention_time[a] > 1) {
			// previous edges
			EXPECT_NEAR(0.3, edge_weights[a], DELTA);
		} else {
			// others
			EXPECT_NEAR(1.0, edge_weights[a], DELTA);
		}
	}
}
/*
TEST_F(SimulatorForPredictionsWithWeights, two_interval_without_combine) {
	simulator.setConfigVariables(1, 1, 1, -1.0, -1, true, 2, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	ASSERT_NEAR(-1.0, simulator.getCombineFactor(), DELTA);
	// the weights of the edges must not change for combine_factor=-1.0
	for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
		EXPECT_NEAR(1.0, edge_weights[a], DELTA);
	}
}

TEST_F(SimulatorForPredictionsWithWeights, three_interval_with_combine_and_lookback) {
	simulator.setConfigVariables(1, 1, 2, 0.3, -1, true, 4, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	ASSERT_NEAR(0.3, simulator.getCombineFactor(), DELTA);
	// the weights of edges are multiplied with 0.7 for current and 0.3 with previous
	for (ListDigraph::ArcIt a(g); a != INVALID; ++a) {
		if (mention_time[a] > 2) {
			// current edges
			EXPECT_NEAR(0.7, edge_weights[a], DELTA);
		} else if (mention_time[a] > 1) {
			// previous edges
			EXPECT_NEAR(0.3, edge_weights[a], DELTA);
		} else {
			// others
			EXPECT_NEAR(1.0, edge_weights[a], DELTA);
		}
	}
}
*/
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
