#include <gtest/gtest.h>

#include "../../main/centrality/simulators/basic_temporal_simulator.h"

namespace {

using namespace lemon;
using namespace std;

class TemporalCentralityTest: public ::testing::Test {
protected:
	ListDigraph g;
	ListDigraph::NodeMap<NodeId> id;
	ListDigraph::ArcMap<long> mention_time;
	EdgeWeightMap edge_weights;
	BasicTemporalSimulator simulator;

	TemporalCentralityTest() :
			id(g), mention_time(g), edge_weights(g), simulator(g, id,
					mention_time, edge_weights, "") {
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

		// [1,2) interval
		ListDigraph::Arc a0 = g.addArc(v0, v1);
		mention_time[a0] = 1;
		edge_weights[a0] = 1.0;
		ListDigraph::Arc a1 = g.addArc(v0, v2);
		mention_time[a1] = 1;
		edge_weights[a1] = 1.0;
		ListDigraph::Arc a2 = g.addArc(v0, v3);
		mention_time[a2] = 1;
		edge_weights[a2] = 1.0;

		// [2,3) interval
		ListDigraph::Arc a3 = g.addArc(v2, v1);
		mention_time[a3] = 2;
		edge_weights[a3] = 1.0;
		ListDigraph::Arc a4 = g.addArc(v2, v4);
		mention_time[a4] = 2;
		edge_weights[a4] = 1.0;
		ListDigraph::Arc a5 = g.addArc(v2, v5);
		mention_time[a5] = 2;
		edge_weights[a5] = 1.0;
		ListDigraph::Arc a6 = g.addArc(v2, v6);
		mention_time[a6] = 2;
		edge_weights[a6] = 1.0;
		ListDigraph::Arc a7 = g.addArc(v2, v6);
		mention_time[a7] = 2;
		edge_weights[a7] = 1.0;
		ListDigraph::Arc a8 = g.addArc(v0, v3);
		mention_time[a8] = 2;
		edge_weights[a8] = 1.0;

		// turn off console logging
		simulator.setLogLevel(false);
	}
};

}

TEST_F(TemporalCentralityTest, all_edge_node_test) {
	EXPECT_EQ(7, countNodes(g));
	EXPECT_EQ(9, countArcs(g));
}

TEST_F(TemporalCentralityTest, current_edge_test) {
	simulator.setConfigVariables(1, 1, 0, -0.1, 1, false, 2, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	vector<long> curr_edges = simulator.getCurrentEdgeChanges();
	ASSERT_EQ(2, curr_edges.size());
	EXPECT_EQ(3, curr_edges[0]);
	EXPECT_EQ(5, curr_edges[1]);
}

TEST_F(TemporalCentralityTest, current_node_test) {
	simulator.setConfigVariables(1, 1, 0, -0.1, 1, false, 2, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	vector<long> curr_nodes = simulator.getCurrentNodeChanges();
	ASSERT_EQ(2, curr_nodes.size());
	EXPECT_EQ(4, curr_nodes[0]);
	EXPECT_EQ(7, curr_nodes[1]);
}

TEST_F(TemporalCentralityTest, edge_changes_test) {
	simulator.setConfigVariables(1, 1, 0, -0.1, 1, false, 2, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	vector<long> edge_changes = simulator.getNewEdgeChanges();
	ASSERT_EQ(2, edge_changes.size());
	EXPECT_EQ(3, edge_changes[0]);
	EXPECT_EQ(4, edge_changes[1]);
}

TEST_F(TemporalCentralityTest, edge_changes_with_multi_test) {
	simulator.setConfigVariables(1, 1, 0, -0.1, 1, true, 2, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	vector<long> edge_changes = simulator.getNewEdgeChanges();
	ASSERT_EQ(2, edge_changes.size());
	EXPECT_EQ(3, edge_changes[0]);
	EXPECT_EQ(5, edge_changes[1]);
}

TEST_F(TemporalCentralityTest, homophily_edge_changes_test) {
	simulator.setConfigVariables(1, 1, 0, -0.1, 1, false, 2, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	vector<long> hom_edge_changes = simulator.getNewHomEdgeChanges();
	ASSERT_EQ(2, hom_edge_changes.size());
	EXPECT_EQ(0, hom_edge_changes[0]);
	EXPECT_EQ(1, hom_edge_changes[1]);
}

TEST_F(TemporalCentralityTest, node_changes_test) {
	simulator.setConfigVariables(1, 1, 0, -0.1, 1, false, 2, 3, false, false,
			false, false, false, false);
	simulator.simulate();
	vector<long> node_changes = simulator.getNewNodeChanges();
	ASSERT_EQ(2, node_changes.size());
	EXPECT_EQ(4, node_changes[0]);
	EXPECT_EQ(3, node_changes[1]);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
