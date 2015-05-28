#include "centrality_json_writer.h"

using namespace std;

void CentralityJsonWriter::setLogLevel(bool enabled) {
	enabled ?
			_logger->setPriority(log4cpp::Priority::INFO) :
			_logger->setPriority(log4cpp::Priority::ERROR);
	_console_logging_enabled = enabled;
}

void CentralityJsonWriter::writeHeader(long start_time, long delta,
		int centrality_lookback_count, int interval_lookback_count,
		bool enable_multi_edges, int maxNumberOfSteps, int topK) {
	stringstream ss;
	ss << topK;

	_outputFile.open(_outputFileName.c_str(), ios::out);

	_outputFile << "{\n" + writeIndent(1) + "\"centrality_test\": {\n";
	_outputFile << writeIndent(2) + "\"start_time\": " << start_time << ",\n";
	_outputFile << writeIndent(2) + "\"delta_time\": " << delta << ",\n";
	_outputFile << writeIndent(2) << std::boolalpha
			<< "\"centrality_prev_interval_count\": "
			<< centrality_lookback_count << ",\n";
	_outputFile << writeIndent(2) + "\"graph_stat_prev_interval_count\": "
			<< interval_lookback_count << ",\n";
	_outputFile << writeIndent(2) << std::boolalpha
			<< "\"enable_multi_edges\": " << enable_multi_edges << ",\n";
	_outputFile << writeIndent(2) + "\"max_num_of_steps:\": "
			<< maxNumberOfSteps << ",\n";
	_outputFile << writeIndent(2) + "\"topK\": " + ss.str() + ",\n";
	_outputFile
			<< writeIndent(2) + "\"measures\": [\n"
					+ writeMeasureSet(_measureSet) + writeIndent(2) + "],\n";
	_outputFile << writeIndent(2) + "\"intervals\": [\n";
	_logger->info("CentralityJsonWriter: header is created.");

}

void CentralityJsonWriter::writeInterval(bool isLastInterval,
		int interval_counter, long interval_start, long interval_stop) {
	_outputFile << writeIndent(3) + "{\n";
	_outputFile << writeIndent(4) + "\"interval\": {\n";

	writeTimeInfoForInterval(interval_start, interval_stop);

	writeGraphStaticticsForInterval();

	writeMeasuresForInterval(interval_counter);

	_outputFile << writeIndent(4) + "}\n";
	_outputFile << writeIndent(3) + "}" + (isLastInterval ? "" : ",") + "\n";
	_logger->info("CentralityJsonWriter: interval %d data was written to file.",
			interval_counter + 1);

}

void CentralityJsonWriter::close() {
	_outputFile << writeIndent(2) + "]\n";
	_outputFile << writeIndent(1) + "}\n}";
	_outputFile.close();
	_logger->info("CentralityJsonWriter: object closed.");
}

void CentralityJsonWriter::writeTimeInfoForInterval(long interval_start,
		long interval_stop) {
	_outputFile << writeIndent(5) + "\"time\": {\n";

	_outputFile << writeIndent(6) + "\"start\": " << interval_start << ",\n";
	_outputFile << writeIndent(6) + "\"stop\": " << interval_stop << "\n";

	_outputFile << writeIndent(5) + "},\n";
}

void CentralityJsonWriter::writeGraphStaticticsForInterval() {
	_outputFile << writeIndent(5) + "\"graph_stat\": {\n";

	// number of current objects
	_outputFile << writeIndent(6) + "\"num_nodes\": "
			<< _stat_counter.numOfCurrentNodes() << ",\n";
	_outputFile << writeIndent(6) + "\"num_edges\": "
			<< _stat_counter.numOfCurrentEdges() << ",\n";

	// number of previous objects
	_outputFile << writeIndent(6) + "\"previous_nodes\": "
			<< _stat_counter.numOfPreviousNodes() << ",\n";

	_outputFile << writeIndent(6) + "\"previous_edges\": "
			<< _stat_counter.numOfPreviousEdges() << ",\n";

	// number of new objects
	_outputFile << writeIndent(6) + "\"new_homophily_edges\": "
			<< _stat_counter.numOfNewHomophilyEdges() << ",\n";
	_outputFile << writeIndent(6) + "\"new_nodes\": "
			<< _stat_counter.numOfNewNodes() << ",\n";
	_outputFile << writeIndent(6) + "\"new_edges\": "
			<< _stat_counter.numOfNewEdges() << ",\n";

	// number of deleted objects
	_outputFile << writeIndent(6) + "\"deleted_nodes\": "
			<< _stat_counter.numOfDeletedNodes() << ",\n";

	_outputFile << writeIndent(6) + "\"deleted_edges\": "
			<< _stat_counter.numOfDeletedEdges() << "\n";

	_outputFile << writeIndent(5) + "},\n";

}

void CentralityJsonWriter::writeMeasuresForInterval(int interval_counter) {
	_outputFile << writeIndent(5) + "\"measures\": {\n";

	if (_centrality_graph.numOfEdges() > 0
			&& _centrality_graph.numOfVertices()
					>= _centrality_graph.getTopK()) {
		TopKSelector topKRecord(0);
		for (unsigned int i = 0; i < _measureSet.size(); i++) {
			if (_measureSet[i] == "beta") {
				topKRecord = _centrality_graph.getTopCentralityMeasureMap(
						_centrality_graph.getFinalBeta());
			}
			if (_measureSet[i] == "pagerank") {
				topKRecord = _centrality_graph.getTopCentralityMeasureMap(
						_centrality_graph.getFinalPageRank());
			}
			if (_measureSet[i] == "loop") {
				topKRecord = _centrality_graph.getTopCentralityMeasureMap(
						_centrality_graph.getFinalRatio());
			}
			if (_measureSet[i] == "harmonic") {
				topKRecord = _centrality_graph.getTopCentralityMeasureMap(
						_centrality_graph.getFinalHarmonicScore());
			}
			if (_measureSet[i] == "salsa_auth") {
				topKRecord = _centrality_graph.getTopSalsaMeasureMap(
						_centrality_graph.getFinalSalsa(), true);
			}
			if (_measureSet[i] == "salsa_hub") {
				topKRecord = _centrality_graph.getTopSalsaMeasureMap(
						_centrality_graph.getFinalSalsa(), false);
			}
			if (_measureSet[i] == "in_degree") {
				topKRecord = _centrality_graph.getTopDegree(
						_centrality_graph.getFinalInEdges());
			}
			if (_measureSet[i] == "out_degree") {
				topKRecord = _centrality_graph.getTopDegree(
						_centrality_graph.getFinalOutEdges());
			}

			writeMeasure(_measureSet[i], topKRecord,
					i == _measureSet.size() - 1);
		}
	} else {
		_logger->error(
				"CentralityJsonWriter: The number_of_nodes=%d in interval %d did not reach top_k=%d, so centrality was not computed for this iteration",
				_centrality_graph.numOfVertices(), interval_counter + 1,  _centrality_graph.getTopK());
	}
	_outputFile << writeIndent(5) + "}\n";
}

void CentralityJsonWriter::writeMeasure(string name, TopKSelector selector,
		bool isLastMeasure) {
	_outputFile << writeIndent(6) + "\"" << name << "\": [\n";
	for (unsigned int i = 0; i < selector.size(); i++) {
		IdValuePair record = selector.getVector()[i];
		writeRecord(record, i + 1, i == selector.size() - 1);
	}
	_outputFile << writeIndent(6) + "]" + (isLastMeasure ? "" : ",") + " \n";
}

void CentralityJsonWriter::writeRecord(IdValuePair record, int position,
		bool isLastRecord) {
	_outputFile << writeIndent(7) + "{\n";
	_outputFile << writeIndent(8) + "\"id\": " << record.id << ",\n";
	_outputFile << writeIndent(8) + "\"value\": " << record.value << ",\n";
	_outputFile << writeIndent(8) + "\"pos\": " << position << "\n";
	_outputFile << writeIndent(7) + "}" + (isLastRecord ? "" : ",") + "\n";
}

string CentralityJsonWriter::writeIndent(int times) {
	string indent("  ");
	string output = "";
	for (int i = 0; i < times; ++i) {
		output += indent;
	}
	return output;
}

string CentralityJsonWriter::writeMeasureSet(vector<string> measureSet) {
	string output = "";
	for (unsigned int i = 0; i < measureSet.size(); ++i) {
		output += writeIndent(3) + "\"" + measureSet[i] + "\"";
		if (i < measureSet.size() - 1) {
			output += ",";
		}
		output += "\n";
	}
	return output;
}
