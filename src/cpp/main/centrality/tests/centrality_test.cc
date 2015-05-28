#include <iostream>
#include <lemon/lgf_reader.h>
#include "../graphs/static_centrality_graph.h"
using namespace std;

int main(int, char** argv)
{

    string input_file=string(argv[1]);
    ListDigraph g;
    ListDigraph::NodeMap<NodeId> id(g);

    try {
      digraphReader(g, input_file). // read the directed graph into g
        nodeMap("label", id). // loading vertexIds
        run();
    } catch (Exception& error) { // check if there was any error
      std::cerr << "Error: " << error.what() << std::endl;
      return -1;
    }

    StaticCentralityGraph cg(g,id,5);

    cout << "###ListDigraphForCentrality###" << endl;

    cout << "We have a directed graph with " << cg.numOfVertices() << " nodes "
         << "and " << cg.numOfEdges() << " arc." << endl;

    cg.computeBetaMeasure();

    return 0;
}
