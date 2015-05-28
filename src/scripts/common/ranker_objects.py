#!/usr/bin/python
# ranking metrics can extract the relevance of a given object with these classes

class ObjectRanker:
	def setOriginal(self, data):
		raise NotImplementedError('subclasses must override setOriginal()!')
	def contains(self, obj):
		raise NotImplementedError('subclasses must override contains()!')
	def getRelevanceForPosition(self,position):
		raise NotImplementedError('subclasses must override getRelevanceForPosition()!')
	def getRelevance(self, obj):
		raise NotImplementedError('subclasses must override getRelevance()!')

class CentralityRanker(ObjectRanker):
	def __init__(self):
		# original data is a ranked list
		self.original=[]

	def setOriginal(self, data):
		if not isinstance(data,list):
			raise TypeError('Original data must be a list!')
		else:
			self.original = data

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

class EdgeRanker(ObjectRanker):
	def __init__(self):
		# original data is an edgelist (map)
		self.original = {} 

	def setOriginal(self, data):
		if not isinstance(data, dict):
			raise TypeError('Original data must be stored in a map as edgelist!')
		else:
			self.original = data

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

