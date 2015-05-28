#!/usr/bin/python

import math
import ranker_objects as ros

metric_list=['ndcg','nrbm','precision']

def computeIdealDCG(ranker_obj, k, version):
	sum = 0.0
	if issubclass(ranker_obj.__class__, ros.ObjectRanker):
		for i in range(0,k):
			if version==1:
				sum += (ranker_obj.getRelevanceForPosition(i+1) / math.log(i+2,2) )
			else:
				sum += ( (math.pow(2, ranker_obj.getRelevanceForPosition(i+1))-1) / math.log(i+2,2) )
	else:
		raise TypeError('ranker_obj must be a subclass of ObjectRanker')
	return sum

def computeIdealRBM(ranker_obj, k, persistance):
	sum = 0.0
	if issubclass(ranker_obj.__class__, ros.ObjectRanker):
		for i in range(0,k):
			sum += ( ranker_obj.getRelevanceForPosition(i+1) * math.pow(persistance,i+1) )
	else:
		raise TypeError('ranker_obj must be a subclass of ObjectRanker')
	return sum * (1-persistance)

def computeRankingDCG(ranker_obj, original, predicted, version):
	ranker_obj.setOriginal(original)
	sum = 0.0
	if issubclass(ranker_obj.__class__,ros.ObjectRanker):
		for i in range(0,len(predicted)):
			item = predicted[i]
			if version==1:
				sum += ( ranker_obj.getRelevance(item) / math.log(i+2,2) )
			else:
				sum += ( (math.pow(2, ranker_obj.getRelevance(item))-1) / math.log(i+2,2) )
	else:
		raise TypeError('ranker_obj must be a subclass of ObjectRanker')
	return sum

def computeRankingRBM(ranker_obj, original, predicted, persistance):
	ranker_obj.setOriginal(original)
	sum = 0.0
	if issubclass(ranker_obj.__class__,ros.ObjectRanker):
		for i in range(0,len(predicted)):
			item = predicted[i]
			sum += ( ranker_obj.getRelevance(item) * math.pow(persistance,i+1) )
	else:
		raise TypeError('ranker_obj must be a subclass of ObjectRanker')
	return sum * (1-persistance)

def computePrecision(ranker_obj, original, predicted):
	ranker_obj.setOriginal(original)
	sum = 0.0
	for i in range(0,len(predicted)):
		item = predicted[i]
		if ranker_obj.contains(item):
			sum += 1
	
	if len(predicted) == 0 :
		return  0.0
	else:
		return sum / len(predicted)

def computeRecall(ranker_obj, original, predicted):
	ranker_obj.setOriginal(original)
	sum = 0.0
	for i in range(0,len(predicted)):
		item = predicted[i]
		if ranker_obj.contains(item):
			sum += 1
	
	if len(original) == 0 :
		return  0.0
	else:
		return sum / len(original)

def computeNDCG(ranker_obj, original, predicted, version):
	if len(predicted) == 0:
		return 0.0
	else:
		return computeRankingDCG(ranker_obj, original, predicted, version) / computeIdealDCG(ranker_obj, len(predicted), version)

def computeNRBM(ranker_obj, original, predicted, persistance):
	if len(predicted) == 0:
		return 0.0
	else:
		return computeRankingRBM(ranker_obj, original, predicted, persistance) / computeIdealRBM(ranker_obj, len(predicted), persistance)

if __name__ == '__main__':
	print "Starting tests..."	

	original=['alma','korte','barack','szilva']
	predicted=['alma','szilva','barack','eper','meggy']
	predicted_1=['dinnye','szilva','barack','eper','meggy']
	predicted_2=['dinnye','szilva','barack','alma','meggy']

	c_ranker = ros.CentralityRanker()

	print computeNDCG(c_ranker,original,original,4,1)
	print computeNDCG(c_ranker,original,original,4,2)
	print computeNRBM(c_ranker,original,original,4,0.85)
	print computePrecision(c_ranker,original,original)
	print computeRecall(c_ranker,original,original)
	print
	print computeNDCG(c_ranker,original,predicted,4,1)
	print computeNDCG(c_ranker,original,predicted,4,2)
	print computeNRBM(c_ranker,original,predicted,4,0.85)
	print computePrecision(c_ranker,original,predicted)
	print computeRecall(c_ranker,original,predicted)
	print	
	print computeNDCG(c_ranker,original,predicted_1,4,1)
	print computeNDCG(c_ranker,original,predicted_1,4,2)
	print computeNRBM(c_ranker,original,predicted_1,4,0.85)
	print computePrecision(c_ranker,original,predicted_1)
	print computeRecall(c_ranker,original,predicted_1)
	print
	print computeNDCG(c_ranker,original,predicted_2,4,1)
	print computeNDCG(c_ranker,original,predicted_2,4,2)
	print computeNRBM(c_ranker,original,predicted_2,4,0.85)
	print computePrecision(c_ranker,original,predicted_2)
	print computeRecall(c_ranker,original,predicted_2)
	print
	print "Tests are finished."	
