#include <gtest/gtest.h>

#include "../../main/centrality/simulators/basic_temporal_simulator.h"

namespace {

using namespace lemon;
using namespace std;

class TemporalCentralityTestWithDeletion: public ::testing::Test {
protected:
	ListDigraph g;
	ListDigraph::NodeMap<NodeId> id;
	ListDigraph::ArcMap<long> mention_time;
	EdgeWeightMap edge_weights;
	BasicTemporalSimulator simulator;

	TemporalCentralityTestWithDeletion() :
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
		edge_weights[a0] = 1.0;
		ListDigraph::Arc a1 = g.addArc(v0, v2);
		mention_time[a1] = 0;
		edge_weights[a1] = 1.0;
		ListDigraph::Arc a15 = g.addArc(v1, v2);
		mention_time[a15] = 0;
		edge_weights[a15] = 1.0;

		// [1,2) interval
		ListDigraph::Arc a2 = g.addArc(v1, v3);
		mention_time[a2] = 1;
		edge_weights[a2] = 1.0;
		ListDigraph::Arc a3 = g.addArc(v1, v4);
		mention_time[a3] = 1;
		edge_weights[a3] = 1.0;
		ListDigraph::Arc a4 = g.addArc(v2, v4);
		mention_time[a4] = 1;
		edge_weights[a4] = 1.0;

		// [2,3) interval
		ListDigraph::Arc a5 = g.addArc(v0, v3);
		mention_time[a5] = 2;
		edge_weights[a5] = 1.0;
		ListDigraph::Arc a6 = g.addArc(v0, v5);
		mention_time[a6] = 2;
		edge_weights[a6] = 1.0;
		ListDigraph::Arc a7 = g.addArc(v0, v6);
		mention_time[a7] = 2;
		edge_weights[a7] = 1.0;
		ListDigraph::Arc a8 = g.addArc(v0, v1);
		mention_time[a8] = 2;
		edge_weights[a8] = 1.0;
		ListDigraph::Arc a9 = g.addArc(v0, v1);
		mention_time[a9] = 2;
		edge_weights[a9] = 1.0;
		ListDigraph::Arc a10 = g.addArc(v1, v4);
		mention_time[a10] = 2;
		edge_weights[a10] = 1.0;
		ListDigraph::Arc a11 = g.addArc(v2, v0);
		mention_time[a11] = 2;
		edge_weights[a11] = 1.0;
		ListDigraph::Arc a12 = g.addArc(v2, v1);
		mention_time[a12] = 2;
		edge_weights[a12] = 1.0;
		ListDigraph::Arc a13 = g.addArc(v0, v5);
		mention_time[a13] = 2;
		edge_weights[a13] = 1.0;
		ListDigraph::Arc a14 = g.addArc(v0, v7);
		mention_time[a14] = 2;
		edge_weights[a14] = 1.0;

		// [3,4) interval
		ListDigraph::Arc a16 = g.addArc(v0, v5);
		mention_time[a16] = 3;
		edge_weights[a16] = 1.0;
		ListDigraph::Arc a17 = g.addArc(v0, v6);
		mention_time[a17] = 3;
		edge_weights[a17] = 1.0;
		ListDigraph::Arc a18 = g.addArc(v8, v9);
		mention_time[a18] = 3;
		edge_weights[a18] = 1.0;
		ListDigraph::Arc a19 = g.addArc(v9, v10);
		mention_time[a19] = 3;
		edge_weights[a19] = 1.0;
		ListDigraph::Arc a20 = g.addArc(v0, v2);
		mention_time[a20] = 3;
		edge_weights[a20] = 1.0;
	}

	virtual void SetUp() {
		// turn off console logging
		simulator.setLogLevel(false);

		simulator.setConfigVariables(0, 1, 0, -0.1, 2, true, 4, 3, false, false,
				false, false, false, false);
		simulator.simulate();
	}
};

}

TEST_F(TemporalCentralityTestWithDeletion, all_edge_node_test) {
	EXPECT_EQ(11, countNodes(g));
	EXPECT_EQ(21, countArcs(g));
}

TEST_F(TemporalCentralityTestWithDeletion, current_edge_test) {
	vector<long> curr_edges = simulator.getCurrentEdgeChanges();
	ASSERT_EQ(4, curr_edges.size());
	EXPECT_EQ(3, curr_edges[0]);
	EXPECT_EQ(3, curr_edges[1]);
	EXPECT_EQ(10, curr_edges[2]);
	EXPECT_EQ(5, curr_edges[3]); // because of enable_multi_edges = true
}

TEST_F(TemporalCentralityTestWithDeletion, current_node_test) {
	vector<long> curr_nodes = simulator.getCurrentNodeChanges();
	ASSERT_EQ(4, curr_nodes.size());
	EXPECT_EQ(3, curr_nodes[0]);
	EXPECT_EQ(4, curr_nodes[1]);
	EXPECT_EQ(8, curr_nodes[2]);
	EXPECT_EQ(7, curr_nodes[3]);
}

TEST_F(TemporalCentralityTestWithDeletion, new_edge_test) {
	vector<long> edge_changes = simulator.getNewEdgeChanges();
	ASSERT_EQ(4, edge_changes.size());
	EXPECT_EQ(3, edge_changes[0]);
	EXPECT_EQ(3, edge_changes[1]);
	EXPECT_EQ(7, edge_changes[2]);
	EXPECT_EQ(3, edge_changes[3]);
}

TEST_F(TemporalCentralityTestWithDeletion, new_homophily_edge_test) {
	vector<long> hom_edge_changes = simulator.getNewHomEdgeChanges();
	ASSERT_EQ(4, hom_edge_changes.size());
	EXPECT_EQ(0, hom_edge_changes[0]);
	EXPECT_EQ(0, hom_edge_changes[1]);
	EXPECT_EQ(3, hom_edge_changes[2]);
	EXPECT_EQ(1, hom_edge_changes[3]);
}

TEST_F(TemporalCentralityTestWithDeletion, new_node_test) {
	vector<long> node_changes = simulator.getNewNodeChanges();
	ASSERT_EQ(4, node_changes.size());
	EXPECT_EQ(3, node_changes[0]);
	EXPECT_EQ(2, node_changes[1]);
	EXPECT_EQ(3, node_changes[2]);
	EXPECT_EQ(3, node_changes[3]);
}

TEST_F(TemporalCentralityTestWithDeletion, previous_edge_test) {
	vector<long> prev_edges = simulator.getPreviousEdgeChanges();
	ASSERT_EQ(4, prev_edges.size());
	EXPECT_EQ(0, prev_edges[0]);
	EXPECT_EQ(3, prev_edges[1]);
	EXPECT_EQ(6, prev_edges[2]);
	EXPECT_EQ(13, prev_edges[3]); // because of enable_multi_edges = true
}

TEST_F(TemporalCentralityTestWithDeletion, previous_node_test) {
	vector<long> prev_nodes = simulator.getPreviousNodeChanges();
	ASSERT_EQ(4, prev_nodes.size());
	EXPECT_EQ(0, prev_nodes[0]);
	EXPECT_EQ(3, prev_nodes[1]);
	EXPECT_EQ(5, prev_nodes[2]);
	EXPECT_EQ(8, prev_nodes[3]);
}

TEST_F(TemporalCentralityTestWithDeletion, deleted_edge_test) {
	vector<long> del_edges = simulator.getDeletedEdgeChanges();
	ASSERT_EQ(4, del_edges.size());
	EXPECT_EQ(0, del_edges[0]);
	EXPECT_EQ(3, del_edges[1]);
	EXPECT_EQ(4, del_edges[2]);
	EXPECT_EQ(10, del_edges[3]); // 2 in the second interval + 8 in the third interval
}

TEST_F(TemporalCentralityTestWithDeletion, deleted_node_test) {
	vector<long> del_nodes = simulator.getDeletedNodeChanges();
	ASSERT_EQ(4, del_nodes.size());
	EXPECT_EQ(0, del_nodes[0]);
	EXPECT_EQ(1, del_nodes[1]);
	EXPECT_EQ(0, del_nodes[2]);
	EXPECT_EQ(4, del_nodes[3]);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
