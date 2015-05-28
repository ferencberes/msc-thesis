#ifndef CENTRALITY_JSON_WRITER_H
#define CENTRALITY_JSON_WRITER_H

#include <iostream>
#include <fstream>
#include "../graphs/temporal_centrality_graph.h"
#include "../utils/temporal_graph_stat_counter.h"

using namespace std;

class CentralityJsonWriter {
public:
	CentralityJsonWriter(string output_folder,
			TemporalCentralityGraph &temporal_cg,
			vector<string> measureSet, TemporalGraphStatCounter &counter,
			bool enable_logging) :
			_centrality_graph(temporal_cg), _stat_counter(counter), _measureSet(measureSet), _console_logging_enabled(
					enable_logging) {

		log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
		layout->setConversionPattern("%d [%p] %m%n");
		log4cpp::Appender *consol_appender = new log4cpp::OstreamAppender(
				"console", &std::cout);
		consol_appender->setLayout(layout);
		_logger = &log4cpp::Category::getInstance(
				std::string("CentralityJsonWriter"));
		_logger->addAppender(consol_appender);
		setLogLevel(_console_logging_enabled);

		// output json file
		string json_name_append = "/intervals.json";
		_outputFileName = output_folder + json_name_append;
		_logger->info("CentralityJsonWriter: result will be written to %s.",
				_outputFileName.c_str());
	}

	void setLogLevel(bool enabled);
	void writeHeader(long start_time, long delta, int centrality_lookback_count,
			int interval_lookback_count, bool enable_multi_edges,
			int maxNumberOfSteps, int topK);
	void writeInterval(bool isLastInterval, int interval_counter,
			long interval_start, long interval_stop);
	void close();
	void writeTimeInfoForInterval(long interval_start, long interval_stop);
	void writeGraphStaticticsForInterval();
	void writeMeasuresForInterval(int interval_counter);
	void writeMeasure(string name, TopKSelector selector, bool isLastMeasure);
	void writeRecord(IdValuePair record, int position, bool isLastRecord);
	string writeIndent(int times);
	string writeMeasureSet(vector<string> measureSet);
private:
	ofstream _outputFile;
	string _outputFileName;
	TemporalCentralityGraph &_centrality_graph;
	TemporalGraphStatCounter &_stat_counter;
	vector<string> _measureSet;
	log4cpp::Category* _logger;
	bool _console_logging_enabled;
};

#endif
