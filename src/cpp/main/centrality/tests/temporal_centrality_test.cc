#include <iostream>
#include <sys/stat.h>
#include <lemon/lgf_reader.h>
#include "../simulators/basic_temporal_simulator.h"

using namespace std;

int main(int argc, char* argv[]) {

	log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
	layout->setConversionPattern("%d [%p] %m%n");

	log4cpp::Appender *consol_appender = new log4cpp::OstreamAppender("console",
			&std::cout);
	consol_appender->setLayout(layout);
	log4cpp::Category& logger = log4cpp::Category::getInstance(
			std::string("temporal_centrality_test_logger"));

	layout->setConversionPattern("%d [%p] %m%n");
	consol_appender->setLayout(layout);

	logger.addAppender(consol_appender);

	if (argc > 2) {
		string input_file = string(argv[1]);
		char* output_folder = argv[2];

		// output folder was created
		mkdir(output_folder, S_IRUSR | S_IWUSR | S_IXUSR);
		logger.info("Output directory created.");

		// the input directed graph
		ListDigraph g;

		// nodes are identified according to NodeId-s
		ListDigraph::NodeMap<NodeId> id(g);

		// mention times are stored for each edge
		ListDigraph::ArcMap<long> mention_time(g);

		try {
			logger.info("Input graph loading in progress...");
			digraphReader(g, input_file). // read the directed graph into g
			nodeMap("label", id). // loading vertexIds
			arcMap("time", mention_time). // at mention time in seconds
			run();
		} catch (Exception& error) { // check if there was any error
			std::cerr << "Error: " << error.what() << std::endl;
			logger.error("Input graph could not be loaded!");
			return -1;
		}
		logger.info("Input graph was loaded successfully.");

		// edge weights can be set from input if necessary
		EdgeWeightMap edge_weights(g, 1.0);

		BasicTemporalSimulator simulator(g, id, mention_time, edge_weights,
				output_folder);

		if (argc > 6) {
			simulator.getGraphInfo();
			simulator.setConfigForSimulation(argc, argv);
			simulator.simulate();
		} else {
			simulator.getTimeInfo();
			simulator.getGraphInfo();
			simulator.showUsage();
		}

	} else {
		logger.warn("Bad syntax: Usage: <input_lgf_file> <output_folder_name>");
	}
	return 0;
}
