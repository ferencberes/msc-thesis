#!/usr/bin/python

import sys, os
import json
from stat import *
from matplotlib import pyplot as plt

sys.path.insert(1, os.path.join(sys.path[0], '..', 'visualization'))
import graph_stat_plotters as myplt

def get_lookback_parameter(json_path):
	f=open(json_path)
	content = json.load(f)
	return content["centrality_test"]["interval_lookback_count"]

def compute_mean_ratios(json_path):
	f=open(json_path)
	data = json.load(f)

	new_edge_ratio = 0
	new_hom_edge_ratio = 0
	new_node_ratio = 0
	
	del_node_ratio = 0
	del_edge_ratio = 0
	
	counter = 0
	for interval in data["centrality_test"]["intervals"]:
		curr_edge_num = interval["interval"]["graph_stat"]["num_edges"]
		curr_node_num = interval["interval"]["graph_stat"]["num_nodes"]

		prev_edge_num = interval["interval"]["graph_stat"]["previous_edges"]
		prev_node_num = interval["interval"]["graph_stat"]["previous_nodes"]

		new_edge_num = interval["interval"]["graph_stat"]["new_edges"]
		new_hom_edge_num = interval["interval"]["graph_stat"]["new_homophily_edges"]
		new_node_num = interval["interval"]["graph_stat"]["new_nodes"]

		del_edge_num = interval["interval"]["graph_stat"]["deleted_edges"]
		del_node_num = interval["interval"]["graph_stat"]["deleted_nodes"]

		new_edge_ratio += (float(new_edge_num) / curr_edge_num)
		new_hom_edge_ratio += (float(new_hom_edge_num) / curr_edge_num)
		new_node_ratio += (float(new_node_num) / curr_node_num)

		if counter != 0:
			del_edge_ratio += (float(del_edge_num) / prev_edge_num)
			del_node_ratio += (float(del_node_num) / prev_node_num)

		counter += 1
	return [new_edge_ratio / counter, new_hom_edge_ratio / counter, new_node_ratio / counter, del_edge_ratio / counter, del_node_ratio / counter]
		

def extract_mean_ratio_plot_data(subdirectory):	
	json_file = os.path.join(subdirectory, 'intervals.json')
	print json_file
	if os.path.isfile(json_file):
		return [get_lookback_parameter(json_file), compute_mean_ratios(json_file)]
	else:
		print 'In directory ' + subdirectory + 	' does not contain intervals.json file!'	


def plotEdgeRatios(new_edge_ratios, new_hom_edge_ratios, del_edge_ratios, plot_dir):    
        myplt.draw_3_plot_from_pairs(new_edge_ratios, new_hom_edge_ratios, del_edge_ratios, plot_dir, 'Edge ratios and interval_lookback_count', 'interval_lookback_count', 'ratio', 'new', 'homophily', 'deleted', 'edge_ratios.png') 

def plotNodeRatios(new_node_ratios, del_node_ratios, plot_dir):    
        myplt.draw_2_plot_from_pairs(new_node_ratios, del_node_ratios, plot_dir, 'Node ratios and interval_lookback_count', 'interval_lookback_count', 'ratio', 'new', 'deleted', 'node_ratios.png') 


###main###
if len(sys.argv) != 2 :
	print 'Usage: <target_dir>'
else:	
	path = sys.argv[1]

	new_node_ratios = []
	new_edge_ratios = []
	new_hom_edge_ratios = []
	del_node_ratios = []
	del_edge_ratios = []

	for dirname in os.listdir(path):
		subdir=os.path.join(path, dirname)		
		if os.path.isdir(subdir):
			subdir_data=extract_mean_ratio_plot_data(subdir)
			new_edge_ratios.append([subdir_data[0],subdir_data[1][0]])
			new_hom_edge_ratios.append([subdir_data[0],subdir_data[1][1]])
			new_node_ratios.append([subdir_data[0],subdir_data[1][2]])
			del_edge_ratios.append([subdir_data[0],subdir_data[1][3]])
			del_node_ratios.append([subdir_data[0],subdir_data[1][4]])

	plotEdgeRatios(new_edge_ratios, new_hom_edge_ratios, del_edge_ratios, sys.argv[1])	
	plotNodeRatios(new_node_ratios, del_node_ratios, sys.argv[1])	

