#include <iostream>
#include <math.h>
#include "../graphs/static_centrality_graph.h"
#include "lemon/list_graph.h"
#include "lemon/lgf_reader.h"

using namespace lemon;
using namespace std;

const float epsilon = 0.00001;
bool diff(float num1, float num2) {
	return fabs(num1-num2)> epsilon;
}

int main(int, char** argv)
{
    string input_file=string(argv[1]);

    cout << "###AcceptenceTest###" << endl;
    cout << input_file << endl;
    try {
      ListDigraph g;
      ListDigraph::NodeMap<NodeId> id(g);
      digraphReader(g, input_file). // read the directed graph into g
        nodeMap("label", id). // loading vertexIds
        run();
      StaticCentralityGraph cg(g,id,5);
      if(cg.numOfVertices()!=6){throw "Number of Nodes does not match!";}
      if(cg.numOfEdges()!=14){throw "Number of Arcs does not match!";}

      cg.printNeighbours();
      if(cg.getNumOfInEdges(0)!=4){throw "Number of inEdges does not match for vertex 0!";}
      if(cg.getNumOfInEdges(5)!=3){throw "Number of inEdges does not match for vertex 5!";}
      if(cg.getNumOfInEdges(1)!=3){throw "Number of inEdges does not match for vertex 1!";}
      if(cg.getNumOfInEdges(3)!=2){throw "Number of inEdges does not match for vertex 3!";}
      if(cg.getNumOfInEdges(4)!=1){throw "Number of inEdges does not match for vertex 4!";}
      if(cg.getNumOfInEdges(2)!=1){throw "Number of inEdges does not match for vertex 2!";}
      if(cg.getNumOfOutEdges(0)!=6){throw "Number of inEdges does not match for vertex 0!";}
      if(cg.getNumOfOutEdges(5)!=0){throw "Number of inEdges does not match for vertex 5!";}
      if(cg.getNumOfOutEdges(1)!=2){throw "Number of inEdges does not match for vertex 1!";}
      if(cg.getNumOfOutEdges(3)!=1){throw "Number of inEdges does not match for vertex 3!";}
      if(cg.getNumOfOutEdges(4)!=2){throw "Number of inEdges does not match for vertex 4!";}
      if(cg.getNumOfOutEdges(2)!=3){throw "Number of inEdges does not match for vertex 2!";}

      cg.computeBetaMeasure();
      if(diff(cg.getBetaScore(0),0.666667)){throw "BetaScore does not match for vertex 0!";}
      if(diff(cg.getBetaScore(5),1.33333)){throw "BetaScore does not match for vertex 5!";}
      if(diff(cg.getBetaScore(4),1.0)){throw "BetaScore does not match for vertex 4!";}
      if(diff(cg.getBetaScore(1),1.0)){throw "BetaScore does not match for vertex 1!";}
      if(diff(cg.getBetaScore(3),0.833333)){throw "BetaScore does not match for vertex 3!";}
      if(diff(cg.getBetaScore(2),0.166667)){throw "BetaScore does not match for vertex 2!";}

      cg.computePageRank(0.85,5);
      if(diff(cg.getPageRank(0),0.0640597)){throw "PageRank does not match for vertex 0!";}
      if(diff(cg.getPageRank(5),0.113111)){throw "PageRank does not match for vertex 5!";}
      if(diff(cg.getPageRank(4),0.0902627)){throw "PageRank does not match for vertex 4!";}
      if(diff(cg.getPageRank(1),0.0815552)){throw "PageRank does not match for vertex 1!";}
      if(diff(cg.getPageRank(3),0.0735566)){throw "PageRank does not match for vertex 3!";}
      if(diff(cg.getPageRank(2),0.0347649)){throw "PageRank does not match for vertex 2!";}

      cg.computeSalsa(5);
      if(diff(cg.getSalsaAuthority(0),1.53941)){throw "SalsaAuthority does not match for vertex 0!";}
      if(diff(cg.getSalsaAuthority(5),1.15323)){throw "SalsaAuthority does not match for vertex 5!";}
      if(diff(cg.getSalsaAuthority(4),1.0)){throw "SalsaAuthority does not match for vertex 4!";}
      if(diff(cg.getSalsaAuthority(1),1.15368)){throw "SalsaAuthority does not match for vertex 1!";}
      if(diff(cg.getSalsaAuthority(3),0.768828)){throw "SalsaAuthority does not match for vertex 3!";}
      if(diff(cg.getSalsaAuthority(2),0.384853)){throw "SalsaAuthority does not match for vertex 2!";}
      if(diff(cg.getSalsaHub(0),2.30912)){throw "SalsaHub does not match for vertex 0!";}
      if(diff(cg.getSalsaHub(5),0.0)){throw "SalsaHub does not match for vertex 5!";}
      if(diff(cg.getSalsaHub(4),0.768808)){throw "SalsaHub does not match for vertex 4!";}
      if(diff(cg.getSalsaHub(1),0.768803)){throw "SalsaHub does not match for vertex 1!";}
      if(diff(cg.getSalsaHub(3),1.0)){throw "SalsaHub does not match for vertex 3!";}
      if(diff(cg.getSalsaHub(2),1.15327)){throw "SalsaHub does not match for vertex 2!";}

      cg.computeLoopEdgeRatio();
      if(diff(cg.getLoopRatio(0),2.0)){throw "LoopEdgeRatio does not match for vertex 0!";}
      if(diff(cg.getLoopRatio(5),0.0)){throw "LoopEdgeRatio does not match for vertex 5!";}
      if(diff(cg.getLoopRatio(4),0.0)){throw "LoopEdgeRatio does not match for vertex 4!";}
      if(diff(cg.getLoopRatio(1),1.0)){throw "LoopEdgeRatio does not match for vertex 1!";}
      if(diff(cg.getLoopRatio(3),0.0)){throw "LoopEdgeRatio does not match for vertex 3!";}
      if(diff(cg.getLoopRatio(2),0.0)){throw "LoopEdgeRatio does not match for vertex 2!";}

      //TODO: this does not match since NodeId is unsigned long int
      cg.computeHarmonic();
      if(diff(cg.getHarmonicScore(0),0.0)){throw "HarmonicScore does not match for vertex 0!";}
      if(diff(cg.getHarmonicScore(5),4.0)){throw "HarmonicScore does not match for vertex 5!";}
      if(diff(cg.getHarmonicScore(4),1.83333)){throw "HarmonicScore does not match for vertex 4!";}
      if(diff(cg.getHarmonicScore(1),2.0)){throw "HarmonicScore does not match for vertex 1!";}
      if(diff(cg.getHarmonicScore(3),2.5)){throw "HarmonicScore does not match for vertex 3!";}
      if(diff(cg.getHarmonicScore(2),1.0)){throw "HarmonicScore does not match for vertex 2!";}

    } catch (Exception& error) { // check if there was any error
      cerr << "Error: " << error.what() << endl;
      return -2;
    } catch (const char* msg) {
      cerr << "Error: " << msg << endl;
      return -1;
    }

    return 0;
}
