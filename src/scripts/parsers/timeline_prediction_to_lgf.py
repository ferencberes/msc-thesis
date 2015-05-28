#!/usr/bin/python

import sys,json
import timeline_tsv_to_lgf as conv

class TimeExtractor:
	def __init__(self, start_t, iv_in_sec):
		self.interval_in_sec = iv_in_sec
		self.start_time = start_t

	def getOrigiTime(self, num):
		num = int(num)
		return str((self.start_time + self.interval_in_sec * num))

if __name__=="__main__":
	if len(sys.argv)==4:
		json_f = open(sys.argv[3],'r')
		json_data = json.load(json_f)
		start_time = json_data["centrality_test"]["start_time"]
		interval_in_sec = json_data["centrality_test"]["delta_time"]
	
		extractor = TimeExtractor(start_time,interval_in_sec)	
		conv.parseTsvToLgf(extractor, sys.argv[1],sys.argv[2],1,2,0)
		json_f.close()
	else:
		print 'Usage: <prediction_file_path> <lgf_file_path> <related_intervals_json>'
