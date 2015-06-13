#!/usr/bin/python

import json, os, sys, copy
import matplotlib.pyplot as plt
import parse_json_output as pjo
import centrality_pred_evaluators as cpe 
import plotter_functions as myplt
import custom_errors as ces

dcg_type = 1
rbm_persistence = 0.7

def convertMapToList(data_map):
	output = []
	for item in data_map:
		output.append([item, data_map[item]])
	return output

def evalPredictionBasedOnLastInterval(json_data, plot_dir, metric_name):
	try:
		if metric_name == 'ndcg':
			computed_metric = cpe.ndcgPredFromLastIntervalForCentrality(json_data, dcg_type)
		elif metric_name == 'nrbm':
			computed_metric = cpe.nrbmPredFromLastIntervalForCentrality(json_data, rbm_persistence)
		elif metric_name == 'precision':	
			computed_metric = cpe.precisionPredFromLastIntervalForCentrality(json_data)
		else:
			raise ces.MetricNameError(metric_name)
		res_metric = computed_metric[1]
		x_args = range(1, computed_metric[0] + 1)
		myplt.draw_line_plots(x_args, convertMapToList(res_metric), 'Prediction based on last interval', 'time intervals', metric_name, 1, 20, 1.0, plot_dir, 'prev_interval_based_centrality_' + metric_name + '.png')
		print metric_name + ' was evaluated on last interval centrality prediction.'
	except ces.MetricNameError as mne:
		print mne.msg

def evalPrediction(prediction_order, json_data_map, plot_dir, metric_name):
	try:
		orig_json_data = json_data_map['previous_pred']
		orig_interval_num = len(orig_json_data["centrality_test"]["intervals"])
		if metric_name == 'ndcg':
			computed_prev = cpe.ndcgPredFromLastIntervalForCentrality(orig_json_data, dcg_type)
		elif metric_name == 'nrbm':
			computed_prev = cpe.nrbmPredFromLastIntervalForCentrality(orig_json_data, rbm_persistence)
		elif metric_name == 'precision':
			computed_prev = cpe.precisionPredFromLastIntervalForCentrality(orig_json_data)
		else:
			raise ces.MetricNameError(metric_name)
	
		x_args = range(1, computed_prev[0] + 1)
		computed_metric_map = {}
		computed_metric_map['previous_pred']=computed_prev
		for prediction in json_data_map:
			if prediction != 'previous_pred':
				pred_json_data = json_data_map[prediction]
				pred_interval_num = len(pred_json_data["centrality_test"]["intervals"])
				if orig_interval_num != pred_interval_num:
					raise ces. IntervalCountError(orig_interval_num, pred_interval_num, prediction)				

				if metric_name == 'ndcg':
					computed_metric = cpe.ndcgPredForCentrality(orig_json_data, pred_json_data, dcg_type)
				elif metric_name == 'nrbm':
					computed_metric = cpe.nrbmPredForCentrality(orig_json_data, pred_json_data, rbm_persistence)
				elif metric_name == 'precision':
					computed_metric = cpe.precisionPredForCentrality(orig_json_data, pred_json_data)
				else:
					raise ces.MetricNameError(metric_name)
				res_metric = computed_metric[1]
				myplt.draw_line_plots(x_args, convertMapToList(res_metric), 'Prediction', 'time intervals', metric_name, 1, 20, 1.0, plot_dir, prediction + '_centrality_' + metric_name + '.png')
				computed_metric_map[prediction]=computed_metric
		generateComparePlots(prediction_order, computed_metric_map, json_data_map['previous_pred'], plot_dir, metric_name)
		print metric_name + ' was evaluated on the given centrality prediction.'
	except ces.MetricNameError as mne:
		print mne.msg
	except ces.IntervalCountError as ice:
		print ice.msg

def generateComparePlots(prediction_order, computed_metric_map, orig_json_data, plot_dir, metric_name):
	res_prev=computed_metric_map['previous_pred'][1]
	x_args = range(1, computed_metric_map['previous_pred'][0] + 1)
	measures = pjo.getMeasureNames(orig_json_data)
	
	for m_name in measures:
		value_list=[]
		for prediction in prediction_order:
			res = computed_metric_map[prediction][1]
			value_list.append([prediction, res[m_name]])
		myplt.draw_line_plots(x_args, value_list, 'Predictions for '+ m_name, 'time intervals', metric_name, 1, 20, 1.0, plot_dir, m_name + '_centrality_' + metric_name +'.png')
	
	statistics_list=[]

	for prediction in prediction_order:
		res = computed_metric_map[prediction][1]
		statistics_list.append([prediction, computeAvgAndScale(measures,res)])
	myplt.draw_error_plots(measures, statistics_list, 'Average, minimum and maximum predictions', 'centrality measures', metric_name, 20, 1.0, plot_dir, 'avg_min_max_centrality_predictions_'+ metric_name +'.png')

def computeAvgAndScale(measure_names, prediction_data):
	avg_metrics=[]
	max_metrics=[]
	min_metrics=[]
	for m_name in measure_names:
		truncated_list = copy.copy(prediction_data[m_name])
		truncated_list.pop(0)
		avg_metrics.append(sum(truncated_list)/len(truncated_list))
		min_metrics.append(min(truncated_list))
		max_metrics.append(max(truncated_list))
	return [avg_metrics,min_metrics,max_metrics]

if __name__=='__main__':
	if len(sys.argv) == 4:
		plot_dir = sys.argv[1]
		metric_name = sys.argv[2]
		orig_json_f = open(sys.argv[3],'r')
		json_data = json.load(orig_json_f)
		orig_json_f.close()
		evalPredictionBasedOnLastInterval(json_data,plot_dir,metric_name)
	elif len(sys.argv) > 4:
		plot_dir = sys.argv[1]
		metric_name = sys.argv[2]
		json_data_map={}
		prediction_order=[]
		orig_json_f = open(sys.argv[3],'r')
		orig_json_data = json.load(orig_json_f)
		json_data_map['previous_pred'] = orig_json_data
		argc = len(sys.argv)
		for i in range (2, argc / 2):	
			pred_json_f = open(sys.argv[2 * i],'r')
			json_data_map[sys.argv[2 * i + 1]] = json.load(pred_json_f)
			prediction_order.append(sys.argv[2 * i + 1])
			pred_json_f.close()
		prediction_order.append('previous_pred')
		
		evalPrediction(prediction_order, json_data_map, plot_dir, metric_name)
	else:
		print 'For centrality prediction based on last interval:'
		print 'Usage: <plot_dir> <metric_name> <original_json_output>' 
		print 'For other centrality predction:'
		print 'Usage: <plot_dir> <metric_name> <original_json_output> <prediction_1_json_output> <pred_1_name> <prediction_2_json_output> <pred_2_name> ...' 

