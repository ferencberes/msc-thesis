#!/usr/bin/python

import ranker_objects as ros

def test_centrality_ranker():
	original = [['alma', 0.25], ['korte', 0.15], ['szilva', 0.12], ['barack',0.08]]	
	c_ranker = ros.CentralityRanker()
	c_ranker.setOriginal(original)
	
	assert 0.25 == c_ranker.getRelevanceForPosition(1)
	assert 0.15 == c_ranker.getRelevanceForPosition(2)
	assert 0.08 == c_ranker.getRelevanceForPosition(4)
	assert 0 == c_ranker.getRelevanceForPosition(100)

	assert True == c_ranker.contains('barack')	
	assert False == c_ranker.contains('dinnye')	
	assert 0.25 == c_ranker.getRelevance('alma')
	assert 0.12 == c_ranker.getRelevance('szilva')
	assert 0 == c_ranker.getRelevance('dinnye')

def test_heuristic_centrality_ranker():
	c_ranker = ros.HeuristicCentralityRanker()
	assert 30 == c_ranker.getRelevanceForPosition(1)
	assert 20 == c_ranker.getRelevanceForPosition(2)
	assert 6 == c_ranker.getRelevanceForPosition(5)
	assert 1 == c_ranker.getRelevanceForPosition(100)

	original = ['alma','korte','szilva','barack']	
	c_ranker.setOriginal(original)
	assert True == c_ranker.contains('barack')	
	assert False == c_ranker.contains('dinnye')	
	assert 30 == c_ranker.getRelevance('alma')
	assert 10 == c_ranker.getRelevance('szilva')
	assert 0 == c_ranker.getRelevance('dinnye')

def test_edge_ranker():
	e_ranker = ros.EdgeRanker()
	assert 1 == e_ranker.getRelevanceForPosition(1)
	assert 1 == e_ranker.getRelevanceForPosition(2000)

	original = {1 : [2,3,4], 2 : [1,5], 4 : [5]}	
	e_ranker.setOriginal(original)
	assert True == e_ranker.contains([1,3])	
	assert True == e_ranker.contains([1,4])	
	assert True == e_ranker.contains([4,5])	
	assert False == e_ranker.contains([5,4])	
	assert False == e_ranker.contains([1,5])	
	assert 1 == e_ranker.getRelevance([1,3])
	assert 1 == e_ranker.getRelevance([2,1])
	assert 0 == e_ranker.getRelevance([5,4])

