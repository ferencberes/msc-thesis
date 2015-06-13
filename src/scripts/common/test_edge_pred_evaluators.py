#!/usr/bin/python

import math
import edge_pred_evaluators as epes
import ranker_objects as ros

epsilon = 0.0001

e_original = {1 : [2,3,4], 2 : [1,5], 4 : [5]}
e_predicted_1 = [[1,2],[2,5],[2,9],[4,1]]
e_predicted_2 = [[1,2],[2,8],[2,9],[4,1],[5,5]]
e_origi_list = [[1,2],[1,3],[1,4],[2,1],[2,5],[4,5]]

e_ranker = ros.EdgeRanker()

def test_precision_pred():
	original_graph = [e_original, e_original, e_original]
	predicted_graph = [[], e_predicted_1, e_predicted_2]
	result = epes.precisionPredForEdges(3, original_graph, predicted_graph)
	assert 0 == result[0]
	assert 0.5 == result[1]
	assert 0.2 == result[2]

def test_ndcg_pred():
	original_graph = [e_original, e_original, e_original]
	predicted_graph = [[], e_origi_list, e_predicted_2]
	result = epes.ndcgPredForEdges(3, original_graph, predicted_graph, 1)
	assert 0 == result[0]
	assert 0.636 - result[1] < epsilon
	assert 0.339 - result[2] < epsilon
		
