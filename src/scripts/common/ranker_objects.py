#!/usr/bin/python
# ranking metrics can extract the relevance of a given object with these classes

class ObjectRanker:
	def setOriginal(self, data):
		raise NotImplementedError('subclasses must override setOriginal()!')
	def getNumOfOriginalItems(self):
		raise NotImplementedError('subclasses must override setOriginal()!')
	def contains(self, obj):
		raise NotImplementedError('subclasses must override contains()!')
	def getRelevanceForPosition(self,position):
		raise NotImplementedError('subclasses must override getRelevanceForPosition()!')
	def getRelevance(self, obj):
		raise NotImplementedError('subclasses must override getRelevance()!')


### CENTRALITY RANKER OBJECTS ###

class CentralityRanker(ObjectRanker):
	def __init__(self):
		# original data is a ranked list
		self.score_map={}
		self.position_map={}

	def setOriginal(self, data):
		if not isinstance(data,list):
			raise TypeError('Original data must be a list!') # format: [[id_1,score_1],[id_2,score_2],..]
		else:
			self.score_map.clear()
			self.position_map.clear()
			i = 1
			for record in data: # the data is ordered
				if (not isinstance(data,list)) or len(record) != 2:
					raise TypeError('Records must be a [item_id, item_score] lists!') 
				self.position_map[i] = record[0]	
				self.score_map[record[0]] = record[1]	
				i += 1

	def getNumOfOriginalItems(self):
		return len(self.position_map)	
	
	def contains(self, obj):
		return obj in self.score_map

	def getRelevanceForPosition(self,position):
		relevance = 0.0
		if position in self.position_map:
			relevance = self.score_map[self.position_map[position]]
		return relevance

	def getRelevance(self, obj):
		relevance = 0.0
		if self.contains(obj):
			relevance = self.score_map[obj]
		return relevance


class HeuristicCentralityRanker(ObjectRanker):
	def __init__(self):
		# original data is a ranked list
		self.original=[]

	def setOriginal(self, data):
		if not isinstance(data,list):
			raise TypeError('Original data must be a list!')
		else:
			self.original = data

	def getNumOfOriginalItems(self):
		return len(self.original)	
	
	def contains(self, obj):
		return obj in self.original

	def getRelevanceForPosition(self,position):
		if position==1:
			relevance=30
		elif position==2:
			relevance=20
		elif position==3:
			relevance=10
		elif position==4:
			relevance=8
		elif position==5:
			relevance=6
		elif position==6:
			relevance=5
		elif position==7:
			relevance=4
		elif position==8:
			relevance=3
		elif position==9:
			relevance=2
		else:
			relevance=1
		return relevance

	def getRelevance(self, obj):
		relevance = 0
		if self.contains(obj):
			relevance = self.getRelevanceForPosition(self.original.index(obj)+1)
		return relevance


### EDGE RANKER OBJECTS ###

class EdgeRanker(ObjectRanker):
	def __init__(self):
		# original data is an edgelist (map)
		self.original = {} 
		self.num_of_original_items = 0.0

	def setOriginal(self, data):
		if not isinstance(data, dict):
			raise TypeError('Original data must be stored in a map as edgelist!')
		else:
			self.original = data
			num = 0.0
			for item in self.original:
				num += len(self.original[item])	
			self.num_of_original_items = num

	def getNumOfOriginalItems(self):
		return self.num_of_original_items 
	
	def contains(self, obj):
		is_element = False
		if isinstance(obj,list) and (len(obj) == 2):
			source = obj[0]	
			target = obj[1]
			if (source in self.original) and target in self.original[source]:
				is_element = True
		else:
			raise TypeError('Edges must have tuple2 format: [source,target]!')
		return is_element

	def getRelevanceForPosition(self,position):
		return 1

	def getRelevance(self, obj):
		relevance = 0
		if self.contains(obj):
			relevance = 1
		return relevance

