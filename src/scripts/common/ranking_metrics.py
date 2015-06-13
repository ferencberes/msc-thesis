import math
import ranker_objects as ros

metric_list=['ndcg','nrbm','precision']

def computeIdealDCG(ranker_obj, original, k, version):
	ranker_obj.setOriginal(original)
	sum = 0.0
	if issubclass(ranker_obj.__class__, ros.ObjectRanker):
		for i in range(0,k):
			if version==1:
				sum += (ranker_obj.getRelevanceForPosition(i+1) / math.log(i+2,2) )
			else:
				sum += ( (math.pow(2, ranker_obj.getRelevanceForPosition(i+1))-1) / math.log(i+2,2) )
	else:
		raise TypeError('ranker_obj must be a subclass of ObjectRanker')
	#print sum
	return sum

def computeIdealRBM(ranker_obj, original, k, persistance):
	ranker_obj.setOriginal(original)
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
	#print sum
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
		res = sum / len(predicted)
		#print res
		return res

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
		return sum / ranker_obj.getNumOfOriginalItems()

def computeNDCG(ranker_obj, original, predicted, version):
	if len(predicted) == 0:
		return 0.0
	else:
		res = computeRankingDCG(ranker_obj, original, predicted, version) / computeIdealDCG(ranker_obj, original, len(predicted), version)
		return res	

def computeNRBM(ranker_obj, original, predicted, persistance):
	if len(predicted) == 0:
		return 0.0
	else:
		res = computeRankingRBM(ranker_obj, original, predicted, persistance) / computeIdealRBM(ranker_obj, original, len(predicted), persistance)
		return res

