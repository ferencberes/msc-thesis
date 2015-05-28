#include <gtest/gtest.h>

#include "../../main/centrality/graphs/static_centrality_graph.h"

namespace {

using namespace lemon;
using namespace std;

const float DELTA = 0.00001;

class StaticCentralityTest: public ::testing::Test {
protected:
	ListDigraph g;
	ListDigraph::NodeMap<NodeId> id;
	StaticCentralityGraph cg;

	StaticCentralityTest() :
			id(g), cg(g, id, 5) {
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

		ListDigraph::Arc a0 = g.addArc(v0, v0);
		ListDigraph::Arc a1 = g.addArc(v0, v0);
		ListDigraph::Arc a2 = g.addArc(v0, v0);
		ListDigraph::Arc a3 = g.addArc(v0, v0);
		ListDigraph::Arc a4 = g.addArc(v0, v1);
		ListDigraph::Arc a5 = g.addArc(v0, v2);
		ListDigraph::Arc a6 = g.addArc(v1, v1);
		ListDigraph::Arc a7 = g.addArc(v1, v5);
		ListDigraph::Arc a8 = g.addArc(v2, v1);
		ListDigraph::Arc a9 = g.addArc(v2, v5);
		ListDigraph::Arc a10 = g.addArc(v2, v3);
		ListDigraph::Arc a11 = g.addArc(v3, v4);
		ListDigraph::Arc a12 = g.addArc(v4, v3);
		ListDigraph::Arc a13 = g.addArc(v4, v5);

		// initialize centrality graph and compute measures
		cg.initializeMaps();
		cg.printNeighbours();
		cg.computeBetaMeasure();
		cg.computePageRank(0.85, 5);
		cg.computeSalsa(5);
		cg.computeLoopEdgeRatio();
		cg.computeHarmonic();
	}
};

}

// this has to be some signed unsignes integer comparison problem...
TEST_F(StaticCentralityTest, edge_node_count) {
	EXPECT_EQ(countNodes(g), 6);
	EXPECT_EQ(countArcs(g), 14);
}

TEST_F(StaticCentralityTest, in_degree_count) {
	EXPECT_EQ(cg.getNumOfInEdges(0), 4);
	EXPECT_EQ(cg.getNumOfInEdges(5), 3);
	EXPECT_EQ(cg.getNumOfInEdges(1), 3);
	EXPECT_EQ(cg.getNumOfInEdges(3), 2);
	EXPECT_EQ(cg.getNumOfInEdges(4), 1);
	EXPECT_EQ(cg.getNumOfInEdges(2), 1);
}

TEST_F(StaticCentralityTest, out_degree_count) {
	EXPECT_EQ(cg.getNumOfOutEdges(0), 6);
	EXPECT_EQ(cg.getNumOfOutEdges(5), 0);
	EXPECT_EQ(cg.getNumOfOutEdges(1), 2);
	EXPECT_EQ(cg.getNumOfOutEdges(3), 1);
	EXPECT_EQ(cg.getNumOfOutEdges(4), 2);
	EXPECT_EQ(cg.getNumOfOutEdges(2), 3);
}

TEST_F(StaticCentralityTest, beta_score) {
	EXPECT_NEAR(cg.getBetaScore(0), 0.666667, DELTA);
	EXPECT_NEAR(cg.getBetaScore(5), 1.33333, DELTA);
	EXPECT_NEAR(cg.getBetaScore(4), 1.0, DELTA);
	EXPECT_NEAR(cg.getBetaScore(1), 1.0, DELTA);
	EXPECT_NEAR(cg.getBetaScore(3), 0.833333, DELTA);
	EXPECT_NEAR(cg.getBetaScore(2), 0.166667, DELTA);
}

TEST_F(StaticCentralityTest, pagerank_score) {
	//cout << cg.getPageRank(0) << endl;
	EXPECT_NEAR(cg.getPageRank(0), 0.0640597, DELTA);
	EXPECT_NEAR(cg.getPageRank(5), 0.113111, DELTA);
	EXPECT_NEAR(cg.getPageRank(4), 0.0902627, DELTA);
	EXPECT_NEAR(cg.getPageRank(1), 0.0815552, DELTA);
	EXPECT_NEAR(cg.getPageRank(3), 0.0735566, DELTA);
	EXPECT_NEAR(cg.getPageRank(2), 0.0347649, DELTA);
}

TEST_F(StaticCentralityTest, salsa_auth_score) {
	EXPECT_NEAR(cg.getSalsaAuthority(0), 1.53941, DELTA);
	EXPECT_NEAR(cg.getSalsaAuthority(5), 1.15323, DELTA);
	EXPECT_NEAR(cg.getSalsaAuthority(4), 1.0, DELTA);
	EXPECT_NEAR(cg.getSalsaAuthority(1), 1.15368, DELTA);
	EXPECT_NEAR(cg.getSalsaAuthority(3), 0.768828, DELTA);
	EXPECT_NEAR(cg.getSalsaAuthority(2), 0.384853, DELTA);
}

TEST_F(StaticCentralityTest, salsa_hub_score) {
	EXPECT_NEAR(cg.getSalsaHub(0), 2.30912, DELTA);
	EXPECT_NEAR(cg.getSalsaHub(5), 0.0, DELTA);
	EXPECT_NEAR(cg.getSalsaHub(4), 0.768808, DELTA);
	EXPECT_NEAR(cg.getSalsaHub(1), 0.768803, DELTA);
	EXPECT_NEAR(cg.getSalsaHub(3), 1.0, DELTA);
	EXPECT_NEAR(cg.getSalsaHub(2), 1.15327, DELTA);
}

TEST_F(StaticCentralityTest, loop_edge_ration) {
	EXPECT_NEAR(cg.getLoopRatio(0), 2.0, DELTA);
	EXPECT_NEAR(cg.getLoopRatio(5), 0.0, DELTA);
	EXPECT_NEAR(cg.getLoopRatio(4), 0.0, DELTA);
	EXPECT_NEAR(cg.getLoopRatio(1), 1.0, DELTA);
	EXPECT_NEAR(cg.getLoopRatio(3), 0.0, DELTA);
	EXPECT_NEAR(cg.getLoopRatio(2), 0.0, DELTA);
}

//TODO: this does not match since NodeId is unsigned long int
TEST_F(StaticCentralityTest, harmonic_score) {
	EXPECT_NEAR(cg.getHarmonicScore(0), 0.0, DELTA);
	EXPECT_NEAR(cg.getHarmonicScore(5), 4.0, DELTA);
	EXPECT_NEAR(cg.getHarmonicScore(4), 1.83333, DELTA);
	EXPECT_NEAR(cg.getHarmonicScore(1), 2.0, DELTA);
	EXPECT_NEAR(cg.getHarmonicScore(3), 2.5, DELTA);
	EXPECT_NEAR(cg.getHarmonicScore(2), 1.0, DELTA);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

