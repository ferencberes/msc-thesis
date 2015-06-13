import json
import ranking_metrics as rm
import ranker_objects as ros
import custom_errors as ces
import parse_json_output as pjo

def extractToplistAndVariables(json_data):
	toplists = pjo.extractToplistsFromJson(json_data)
	measure_name_list = toplists[0]
	measure_toplists = toplists[1]
	num_of_intervals = len(json_data["centrality_test"]["intervals"])
	top_k = json_data["centrality_test"]["topK"]
	return [measure_name_list, measure_toplists, num_of_intervals, top_k]

# Centrality evalutator functions
def ndcgPredFromLastIntervalForCentrality(orig_json_data, version):
	params = [version]
	return predFromLastIntervalForCentrality(orig_json_data, 'ndcg', params)

def nrbmPredFromLastIntervalForCentrality(orig_json_data, persistance):
	params = [persistance]
	return predFromLastIntervalForCentrality(orig_json_data, 'nrbm', params)

def precisionPredFromLastIntervalForCentrality(orig_json_data):
	return predFromLastIntervalForCentrality(orig_json_data, 'precision', [])

def predFromLastIntervalForCentrality(orig_json_data, metric_name, params):
	[measure_name_list, orig_measure_toplists, orig_num_of_intervals, orig_top_k] = extractToplistAndVariables(orig_json_data)
	c_ranker = ros.CentralityRanker()
	computed_metric_map = {}
	try:
		for m_name in measure_name_list:
			computed_metric_map[m_name] = []	
			computed_metric_map[m_name].append(0) # there is no first prediction

		for m_name in measure_name_list:
			for i in range(1, orig_num_of_intervals):
				original = orig_measure_toplists[m_name][i]
				prediction = []
				for item in orig_measure_toplists[m_name][i-1]:
					prediction.append(item[0])
				if not isinstance(params,list):
					raise TypeError('params variable must be a list!')
				if metric_name == 'ndcg':
					if len(params) == 0:
						raise IndexError('params must contain DCG version id!')	
					else:
						version = params[0]
						computed_metric_map[m_name].append(rm.computeNDCG(c_ranker, original, prediction, version))
				elif metric_name == 'nrbm':
					if len(params) == 0:
						raise IndexError('params must contain persistance parameter for RBM!')	
					else:
						persistance = params[0]
						computed_metric_map[m_name].append(rm.computeNRBM(c_ranker, original, prediction, persistance))

				elif metric_name == 'precision':
					computed_metric_map[m_name].append(rm.computePrecision(c_ranker, original, prediction))
				else:
					raise MetricNameError(metric_name) 
	except TypeError as te:
		print te.msg
	except ces.MetricNameError as mne:
		print mne.msg
	except IndexError as ie:
		print ie.msg
	finally:
		#print computed_metric_map
		return [orig_num_of_intervals,computed_metric_map]

def ndcgPredForCentrality(orig_json_data, pred_json_data, version):
	params = [version]
	return predForCentrality(orig_json_data, pred_json_data, 'ndcg', params)

def nrbmPredForCentrality(orig_json_data, pred_json_data, persistance):
	params = [persistance]
	return predForCentrality(orig_json_data, pred_json_data, 'nrbm', params)

def precisionPredForCentrality(orig_json_data, pred_json_data):
	return predForCentrality(orig_json_data, pred_json_data, 'precision', [])

def predForCentrality(orig_json_data, pred_json_data, metric_name, params):
	[measure_name_list, orig_measure_toplists, orig_num_of_intervals, orig_top_k] = extractToplistAndVariables(orig_json_data)
	[pred_measure_name_list, pred_measure_toplists, pred_num_of_intervals, pred_top_k] = extractToplistAndVariables(pred_json_data)
	c_ranker = ros.CentralityRanker()
	computed_metric_map = {}
	try:
		if(orig_top_k != pred_top_k):
			raise ces.TopKError(orig_top_k,pred_top_k)
		if(orig_num_of_intervals != pred_num_of_intervals):
			raise ces.IntervalCountError(orig_num_of_intervals,pred_num_of_intervals)
		
		for m_name in measure_name_list:
			computed_metric_map[m_name] = []	
			computed_metric_map[m_name].append(0) # there is no first prediction

		for m_name in measure_name_list:
			for i in range(1, orig_num_of_intervals):
				original = orig_measure_toplists[m_name][i]
				prediction = []
				for item in pred_measure_toplists[m_name][i]:
					prediction.append(item[0])
				if not isinstance(params,list):
					raise TypeError('params variable must be a list!')
				if metric_name == 'ndcg':
					if len(params) == 0:
						raise IndexError('params must contain DCG version id!')	
					else:
						version = params[0]
						computed_metric_map[m_name].append(rm.computeNDCG(c_ranker, original, prediction, version))
				elif metric_name == 'nrbm':
					if len(params) == 0:
						raise IndexError('params must contain persistance parameter for RBM!')	
					else:
						persistance = params[0]
						computed_metric_map[m_name].append(rm.computeNRBM(c_ranker, original, prediction, persistance))
				elif metric_name == 'precision':
					computed_metric_map[m_name].append(rm.computePrecision(c_ranker, original, prediction))
				else:
					raise ces.MetricNameError(metric_name) 
	except ces.TopKError as tke:
		print tke.msg
	except ces.IntervalCountError as ice:
		print ice.msg
	except TypeError as te:
		print te.msg
	except ces.MetricNameError as mne:
		print mne.msg
	except IndexError as ie:
		print ie.msg
	finally:
		#print computed_metric_map
		return [orig_num_of_intervals,computed_metric_map]


