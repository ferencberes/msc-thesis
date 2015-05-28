#!/usr/bin/python

import sys
import os
import json
import numpy as np
from matplotlib import dates
import datetime

sys.path.insert(1, os.path.join(sys.path[0], '..', 'common'))
import plotter_functions as myplt

def convertEpochToNum(epoch_vector):
	dts = map(datetime.datetime.fromtimestamp,epoch_vector)
	nums = dates.date2num(dts)
	return nums	

def plotEdgesAndNodes(x_args, num_edges, num_nodes, plot_dir):
	plot_data = []
	plot_data.append(['#edges', num_edges])
	plot_data.append(['#nodes', num_nodes])
	myplt.draw_plots_with_time(x_args, plot_data, 'Number of edges and nodes', 'date', '', 3, plot_dir, 'edges_and_nodes.png')	

def plotNumberOfNewNodes(x_args, time_vector, num_nodes, new_nodes, plot_dir):
	plot_data = []
	plot_data.append(['current', num_nodes])
	plot_data.append(['new', new_nodes])
	myplt.draw_line_plots(x_args, plot_data, 'Number of new nodes', 'time intervals', 'node count', 1, -1, 0, plot_dir, 'new_nodes.png')	

def plotNumberOfDeletedNodes(x_args, time_vector, prev_nodes, del_nodes, plot_dir):
	plot_data = []
	plot_data.append(['previous', prev_nodes])
	plot_data.append(['deleted', del_nodes])
	myplt.draw_line_plots(x_args, plot_data, 'Number of deleted nodes', 'time intervals', 'node count', 1, -1, 0, plot_dir, 'deleted_nodes.png')	

def plotNumberOfNewEdges(x_args, time_vector, num_edges, new_edges, hom_edges, plot_dir):
	plot_data = []
	plot_data.append(['current', num_edges])
	plot_data.append(['new', new_edges])
	plot_data.append(['homophily', hom_edges])
	myplt.draw_line_plots(x_args, plot_data, 'Number of new and homophily edges', 'time intervals', 'edge count', 1, -1, 0, plot_dir, 'new_edges.png')	

def plotNumberOfDeletedEdges(x_args, time_vector, prev_edges, del_edges, plot_dir):
	plot_data = []
	plot_data.append(['previous', prev_edges])
	plot_data.append(['deleted', del_edges])
	myplt.draw_line_plots(x_args, plot_data, 'Number of deleted edges', 'time intervals', 'edge count', 1, -1, 0, plot_dir, 'deleted_edges.png')	

###main###
if len(sys.argv) != 3 :
	print 'Usage: <centrality_log.json> <output_folder>'
else:
	json_data=open(sys.argv[1])
	plot_dir=sys.argv[2]

	if not os.path.isdir(plot_dir):
   		os.makedirs(plot_dir)

	data = json.load(json_data)
	json_data.close()

	start_times = []
	stop_times = []
	num_nodes = []
	num_edges = []
	new_nodes = []
	new_edges = []
	homopily_edges = []
	prev_nodes = []
	prev_edges = []
	del_nodes = []
	del_edges = []
	
	for interval in data["centrality_test"]["intervals"]:
		start_times.append(interval["interval"]["time"]["start"])
		stop_times.append(interval["interval"]["time"]["stop"])
		num_nodes.append(interval["interval"]["graph_stat"]["num_nodes"])
		num_edges.append(interval["interval"]["graph_stat"]["num_edges"])
		new_nodes.append(interval["interval"]["graph_stat"]["new_nodes"])
		new_edges.append(interval["interval"]["graph_stat"]["new_edges"])
		prev_nodes.append(interval["interval"]["graph_stat"]["previous_nodes"])
		prev_edges.append(interval["interval"]["graph_stat"]["previous_edges"])
		del_nodes.append(interval["interval"]["graph_stat"]["deleted_nodes"])
		del_edges.append(interval["interval"]["graph_stat"]["deleted_edges"])
		homopily_edges.append(interval["interval"]["graph_stat"]["new_homophily_edges"])

	start_times = np.array(start_times)
	stop_times = np.array(stop_times)
	start_times = convertEpochToNum(start_times)

	x_args = range(1,len(start_times) + 1)
	plotEdgesAndNodes(start_times, num_edges, num_nodes, plot_dir)

	plotNumberOfNewEdges(x_args, start_times, num_edges, new_edges, homopily_edges, plot_dir)
	plotNumberOfDeletedEdges(x_args, start_times, prev_edges, del_edges, plot_dir)
	
	plotNumberOfNewNodes(x_args, start_times, num_nodes, new_nodes, plot_dir)
	plotNumberOfDeletedNodes(x_args, start_times, prev_nodes, del_nodes, plot_dir)
	
print "Graph plots were created."
