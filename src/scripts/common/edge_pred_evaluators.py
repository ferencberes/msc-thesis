import json
import ranking_metrics as rm
import ranker_objects as ros
import custom_errors as ces

# Centrality evalutator functions
def ndcgPredForEdges(num_of_intervals, orig_edges, predicted_edges, version):
	params = [version]
	return predForEdges(num_of_intervals, orig_edges, predicted_edges,'ndcg', params)

def nrbmPredForEdges(num_of_intervals, orig_edges, predicted_edges, persistance):
	params = [persistance]
	return predForEdges(num_of_intervals, orig_edges, predicted_edges, 'nrbm', params)

def precisionPredForEdges(num_of_intervals, orig_edges, predicted_edges):
	return predForEdges(num_of_intervals, orig_edges, predicted_edges, 'precision', [])

def predForEdges(num_of_intervals, orig_edges, predicted_edges, metric_name, params):
	e_ranker = ros.EdgeRanker()
	computed_metric_list = [] 
	try:
		computed_metric_list.append(0) # there is no first prediction
		# TODO: check whether interval length equals!!!
		for i in range(1, num_of_intervals):
			#print i
			original = orig_edges[i] # this is a map
			prediction = predicted_edges[i] # this is a list
			if not isinstance(params,list):
				raise TypeError('params variable must be a list!')
			if metric_name == 'ndcg':
				if len(params) == 0:
					raise IndexError('params must contain DCG version id!')	
				else:
					version = params[0]
					computed_metric_list.append(rm.computeNDCG(e_ranker, original, prediction, version))
			elif metric_name == 'nrbm':
				if len(params) == 0:
					raise IndexError('params must contain persistance parameter for RBM!')	
				else:
					persistance = params[0]
					computed_metric_list.append(rm.computeNRBM(e_ranker, original, prediction, persistance))
			elif metric_name == 'precision':
				computed_metric_list.append(rm.computePrecision(e_ranker, original, prediction))
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
		#print computed_metric_list
		return computed_metric_list


