import os,sys
import eval_edge_prediction as eep

def test_load_edge_data():
	num_of_intervals = 3
        edge_reader = eep.OriginalEdgeReader()
	test_folder_path = os.path.join(sys.path[0], '../../../resources/example/data_for_tests')
        origi_edges = eep.loadEdgeData(num_of_intervals, test_folder_path, edge_reader, 'interval_edges_')
	assert 3 == len(origi_edges)
	# first interval 
	assert 3 == len(origi_edges[0])
	assert True == ('2' in origi_edges[0]['1'])
	assert True == ('3' in origi_edges[0]['1'])
	assert True == ('3' in origi_edges[0]['2'])
	assert True == ('3' in origi_edges[0]['4'])
	assert True == ('1' in origi_edges[0]['2'])
	# second interval
	assert 4 == len(origi_edges[1])
	assert True == ('5' in origi_edges[1]['2'])
	assert True == ('3' in origi_edges[1]['5'])
	assert True == ('2' in origi_edges[1]['4'])
	assert True == ('2' in origi_edges[1]['9'])
	# third interval 
	assert 3 == len(origi_edges[2])
	assert True == ('1' in origi_edges[2]['8'])
	assert True == ('6' in origi_edges[2]['5'])
	assert True == ('7' in origi_edges[2]['6'])

def test_load_prediction_edge_data():
	num_of_intervals = 3
	pred_edges_toplist_path = os.path.join(sys.path[0], '../../../resources/example/data_for_tests/predicted_edges.gtl')
        pred_edges = eep.loadPredictionEdgeData(num_of_intervals, pred_edges_toplist_path)
	assert 3 == len(pred_edges)
	assert 0 == len(pred_edges[0]) # there is no prediction for the first interval!
	# second interval predictions
	assert 4 == len(pred_edges[1])
	assert True == (['2','5'] in pred_edges[1])
	assert True == (['5','3'] in pred_edges[1])
	assert True == (['4','2'] in pred_edges[1])
	assert True == (['9','2'] in pred_edges[1])
	# third interval predictions
	assert 5 == len(pred_edges[2])
	assert True == (['10','1'] in pred_edges[2])
	assert True == (['8','1'] in pred_edges[2])
	assert True == (['5','6'] in pred_edges[2])
	assert True == (['6','7'] in pred_edges[2])
	assert True == (['7','5'] in pred_edges[2])

