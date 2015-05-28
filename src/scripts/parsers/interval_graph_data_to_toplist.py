#!/usr/bin/python

import sys

# This script can write the homophily or other edges to a global toplist file
if len(sys.argv) == 6:
	graph_data_folder_path = sys.argv[1]
	file_prefix = sys.argv[2]
	out_f = open(sys.argv[5], 'w')
	start_iv_counter = int(sys.argv[3])
	stop_iv_counter = int(sys.argv[4])
	for i in range(start_iv_counter, stop_iv_counter + 1):
		interval_file_path = graph_data_folder_path + '/' + file_prefix + '_' + str(i)
		interval_file = open(interval_file_path, 'r')
		for line in interval_file:
			out_f.write(line)
		interval_file.close()
	out_f.close()
	print 'Toplist file was generated.'	
else:
	print 'Usage: <graph_data_folder> <file_prefix> <from> <to> <output_path>'

