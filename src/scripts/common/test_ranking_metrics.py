#!/usr/bin/python
# TODO: check NDCG version 2. Check RBM related functions!!!

import math
import ranking_metrics as rms
import ranker_objects as ros

epsilon = 0.0001

c_original = [['alma',0.25], ['korte',0.15], ['szilva',0.12], ['barack',0.08]]

hc_original = ['alma','korte','szilva','barack']
hc_pred_for_recall = ['alma','szilva','barack','meggy','korte','avokado']
hc_predicted = ['alma','szilva','barack','eper','meggy']
hc_predicted_1 = ['dinnye','szilva']

e_original = {1 : [2,3,4], 2 : [1,5], 4 : [5]}
e_predicted = [[1,2],[2,5],[2,9],[4,1]]
e_predicted_1 = [[1,2],[2,8],[2,9],[4,1],[5,5]]
e_origi_list = [[1,2],[1,3],[1,4],[2,1],[2,5],[4,5]]

c_ranker = ros.CentralityRanker()
hc_ranker = ros.HeuristicCentralityRanker()
e_ranker = ros.EdgeRanker()

def test_c_precision():
	assert 1 == rms.computePrecision(c_ranker, c_original, hc_original)
	assert 0.6 == rms.computePrecision(c_ranker, c_original, hc_predicted)
	assert 0.5 == rms.computePrecision(c_ranker, c_original, hc_predicted_1)

def test_hc_precision():
	assert 1 == rms.computePrecision(hc_ranker, hc_original, hc_original)
	assert 0.6 == rms.computePrecision(hc_ranker, hc_original, hc_predicted)
	assert 0.5 == rms.computePrecision(hc_ranker, hc_original, hc_predicted_1)
	
def test_e_precision():
	assert 1 == rms.computePrecision(e_ranker, e_original, e_origi_list)
	assert 0.5 == rms.computePrecision(e_ranker, e_original, e_predicted)
	assert 0.2 == rms.computePrecision(e_ranker, e_original, e_predicted_1)
		
def test_c_recall():
	assert 1 == rms.computeRecall(c_ranker, c_original, hc_pred_for_recall)
	assert 0.75 == rms.computeRecall(c_ranker, c_original, hc_predicted)
	assert 0.25 == rms.computeRecall(c_ranker, c_original, hc_predicted_1)

def test_hc_recall():
	assert 1 == rms.computeRecall(hc_ranker, hc_original, hc_pred_for_recall)
	assert 0.75 == rms.computeRecall(hc_ranker, hc_original, hc_predicted)
	assert 0.25 == rms.computeRecall(hc_ranker, hc_original, hc_predicted_1)
	
def test_e_recall():
	assert 1 == rms.computeRecall(e_ranker, e_original, e_origi_list)
	assert 1.0 / 3 == rms.computeRecall(e_ranker, e_original, e_predicted)
	assert 1.0 / 6 == rms.computeRecall(e_ranker, e_original, e_predicted_1)

def test_c_dcg():
	assert abs(0.4390 - rms.computeRankingDCG(c_ranker, c_original, hc_original, 1)) < epsilon
	assert abs(0.3657 - rms.computeRankingDCG(c_ranker, c_original, hc_predicted, 1)) < epsilon
	assert abs(0.0757 - rms.computeRankingDCG(c_ranker, c_original, hc_predicted_1, 1)) < epsilon

def test_hc_dcg():
	assert abs(51.0640 - rms.computeRankingDCG(hc_ranker, hc_original, hc_original, 1)) < epsilon
	assert abs(40.3092 - rms.computeRankingDCG(hc_ranker, hc_original, hc_predicted, 1)) < epsilon
	assert abs(6.3092 - rms.computeRankingDCG(hc_ranker, hc_original, hc_predicted_1, 1)) < epsilon

def test_e_dcg():
	assert abs(3.3046 - rms.computeRankingDCG(e_ranker, e_original, e_origi_list, 1)) < epsilon
	assert abs(1.6309 - rms.computeRankingDCG(e_ranker, e_original, e_predicted, 1)) < epsilon
	assert abs(1.0 - rms.computeRankingDCG(e_ranker, e_original, e_predicted_1, 1)) < epsilon

def test_c_ndcg():
	assert abs(1.0 - rms.computeNDCG(c_ranker, c_original, hc_original, 1)) < epsilon
	assert abs(0.8328 - rms.computeNDCG(c_ranker, c_original, hc_predicted, 1)) < epsilon # here IDCG computed for #predicted_items = 5
	assert abs(0.2196 - rms.computeNDCG(c_ranker, c_original, hc_predicted_1, 1)) < epsilon # here IDCG computed for #predicted_items = 2

def test_hc_ndcg():
	assert abs(1.0 - rms.computeNDCG(hc_ranker, hc_original, hc_original, 1)) < epsilon
	assert abs(0.7550 - rms.computeNDCG(hc_ranker, hc_original, hc_predicted, 1)) < epsilon # here IDCG computed for #predicted_items = 5
	assert abs(0.1480 - rms.computeNDCG(hc_ranker, hc_original, hc_predicted_1, 1)) < epsilon # here IDCG computed for #predicted_items = 2 

def test_e_ndcg():
	assert abs(1.0 - rms.computeNDCG(e_ranker, e_original, e_origi_list, 1)) < epsilon
	assert abs(0.6366 - rms.computeNDCG(e_ranker, e_original, e_predicted, 1)) < epsilon # here IDCG computed for #predicted_items = 4
	assert abs(0.3391 - rms.computeNDCG(e_ranker, e_original, e_predicted_1, 1)) < epsilon # here IDCG computed for #predicted_items = 5

