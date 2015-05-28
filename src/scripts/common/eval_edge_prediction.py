#!/usr/bin/python

import json, os, sys, copy
import matplotlib.pyplot as plt
import parse_json_output as pjo
import edge_pred_evaluators as epe
import plotter_functions as myplt
import custom_errors as ces

dcg_type = 1
rbm_persistence = 0.7

class EdgeReader:
	def loadEdgesFromFile(self,file_path):
		raise NotImplementedError('subclasses must override loadEdgesFromFile()!')

class OriginalEdgeReader(EdgeReader):
	def loadEdgesFromFile(self,file_path):
		edges = {}
		f = open(file_path,'r')
		for edge in f:
			split_line = edge.split()
			source = split_line[1]
			target = split_line[2]
			if source not in edges:
				edges[source]=[]
			# TODO: it is supposed that there are no multiple edges!
			edges[source].append(target)
		f.close()
		return edges

def loadEdgeData(num_of_intervals, edge_data_folder, edge_reader, file_prefix):
	interval_list = []
	for i in range(0,num_of_intervals):
		file_path = edge_data_folder + '/' + file_prefix + str(i)
		interval_list.append(edge_reader.loadEdgesFromFile(file_path))
	return interval_list	

def loadPredictionEdgeData(num_of_intervals, global_toplist_file):
	interval_list = []
	edge_map = {}
	for i in range(0,num_of_intervals):
		edge_map[str(i)]=[]
	pred_file = open(global_toplist_file,'r')
	for edge in pred_file:
		split_line = edge.split()
		edge_to_insert = [split_line[1],split_line[2]]
		# read prediction only until test interval number
		if int(split_line[0]) < num_of_intervals:
			edge_map[split_line[0]].append(edge_to_insert)
	pred_file.close()
	for i in range(0,num_of_intervals):
		interval_list.append(edge_map[str(i)])
	return interval_list	

def evalAllEdgePrediction(prediction_order, orig_json_data, edge_data_map, metric_name):
	num_of_intervals = len(orig_json_data["centrality_test"]["intervals"])
	edge_reader = OriginalEdgeReader()
	origi_edges = loadEdgeData(num_of_intervals, edge_data_map['original'], edge_reader, 'interval_edges_')
	evalPrediction(prediction_order, orig_json_data, edge_data_map, metric_name, origi_edges, 'all')

def evalHomophilyEdgePrediction(prediction_order, orig_json_data, edge_data_map, metric_name):
	num_of_intervals = len(orig_json_data["centrality_test"]["intervals"])
	edge_reader = OriginalEdgeReader()
	homophily_edges = loadEdgeData(num_of_intervals, edge_data_map['original'], edge_reader, 'interval_homophily_edges_')
	evalPrediction(prediction_order, orig_json_data, edge_data_map, metric_name, homophily_edges, 'homophily')

def evalPrediction(prediction_order, orig_json_data, edge_data_map, metric_name, edges, prefix):
	num_of_intervals = len(orig_json_data["centrality_test"]["intervals"])
	prediction_metric_map = {}
	try:
		for prediction in edge_data_map:
			if not prediction == 'original':
				pred_edges = loadPredictionEdgeData(num_of_intervals, edge_data_map[prediction])
				if len(edges) != len(pred_edges):
					raise ces.IntervalCountError(len(edges), len(pred_edges))				

				if metric_name == 'ndcg':
					computed_metric = epe.ndcgPredForEdges(num_of_intervals, edges, pred_edges, dcg_type)
				elif metric_name == 'nrbm':
					computed_metric = epe.nrbmPredForEdges(num_of_intervals, edges, pred_edges, rbm_persistence)
				elif metric_name == 'precision':
					computed_metric = epe.precisionPredForEdges(num_of_intervals, edges, pred_edges)
				else:
					raise ces.MetricNameError(metric_name)
				prediction_metric_map[prediction] = computed_metric
		x_args = range(1,num_of_intervals + 1)
		generateComparePlots(prediction_order, num_of_intervals, prediction_metric_map, plot_dir, metric_name, prefix)
		print metric_name + ' was evaluated for ' + prefix + '  link predictions.'
	except ces.MetricNameError as mne:
		print mne.msg
	except ces.IntervalCountError as ice:
                print ice.msg

def generateComparePlots(prediction_order, num_of_intervals, prediction_metric_map, plot_dir, metric_name, prefix):
	x_args = range(1, num_of_intervals + 1)
	value_list = []
	statistics_list=[]
	for prediction in prediction_order:
		res = prediction_metric_map[prediction]
		value_list.append([prediction, res])
		statistics_list.append([prediction, computeAvgAndScale(res)])
	
	if prefix == 'homophily':
		ylim = 0.15
	else:
		ylim = 0.25

	myplt.draw_line_plots(x_args, value_list, 'Link prediction for ' + prefix + ' edges', 'time intervals', metric_name, 1, 20, ylim, plot_dir, prefix + '_link_prediction_' + metric_name + '.png')
	myplt.draw_error_plots([''], statistics_list, 'Average, minimum and maximum for ' + prefix + ' link predictions', 'time intervals', metric_name, 20, ylim, plot_dir, prefix + '_avg_min_max_link_predictions_'+ metric_name +'.png')

def computeAvgAndScale(prediction_data):
	avg_metrics=[]
	max_metrics=[]
	min_metrics=[]
	truncated_list = copy.copy(prediction_data)
	truncated_list.pop(0)
	avg_metrics.append(sum(truncated_list)/len(truncated_list))
	min_metrics.append(min(truncated_list))
	max_metrics.append(max(truncated_list))
	return [avg_metrics,min_metrics,max_metrics]

if __name__=='__main__':
	if len(sys.argv) >= 7:
		plot_dir = sys.argv[1]
		metric_name = sys.argv[2]
		edge_data_map={}
		prediction_order = []

		orig_json_f = open(sys.argv[3],'r')
		orig_json_data = json.load(orig_json_f)
		orig_json_f.close()
		edge_data_map['original'] = sys.argv[4]		

		argc = len(sys.argv) + 1
		for i in range (3, argc / 2):	
			edge_data_map[sys.argv[2 * i]] = sys.argv[2 * i - 1]
			prediction_order.append(sys.argv[2 * i])	

		evalAllEdgePrediction(prediction_order, orig_json_data, edge_data_map, metric_name)	
		evalHomophilyEdgePrediction(prediction_order, orig_json_data, edge_data_map, metric_name)	
	else:
		print 'Usage: <plot_dir> <metric_name> <original_json_output> <original_edges_folder> <prediction_1_output> <pred_1_name> <prediction_2_output> <pred_2_name> ...' 

