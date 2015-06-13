#!/usr/bin/python

import math
import centrality_pred_evaluators as cpes
import ranker_objects as ros

epsilon = 0.0001

c_original = [['alma',0.25], ['korte',0.15], ['szilva',0.12], ['barack',0.08]]
hc_original = ['alma','korte','szilva','barack']
hc_predicted_1 = ['alma','szilva','barack','eper','meggy']
hc_predicted_2 = ['dinnye','szilva']

c_ranker = ros.CentralityRanker()

def test_precision_pred():
	original_nodes = [c_original, c_original, c_original]
	#predicted_nodes = [[], hc_predicted_1, hc_predicted_2]
	#result = cpes.precisionPredForCentrality(3, original_nodes, predicted_nodes)
	#assert 0 == result[0]
	#assert 0.6 == result[1]
	#assert 0.5 == result[2]

def test_precision_baseline_pred():
	original_nodes = [c_original, c_original, c_original]
	#result = cpes.precisionPredForCentrality(3, original_nodes, predicted_nodes)
	#assert 0 == result[0]
	#assert 0.6 == result[1]
	#assert 0.5 == result[2]

		
