#!/usr/bin/python

import os,sys,json

class IntervalEdgeList:
	def __init__(self, interval_id, output_folder):
		self.interval_id = interval_id
		self.output_folder = output_folder
		self.edge_list={}
	
	def clear(self):
		self.edge_list.clear()
	
	def setIntervalId(self, new_id):
		self.interval_id=new_id
	
	def addEdge(self, source, target):
		if source in self.edge_list:
			self.edge_list[source].append(target)
		else:
			self.edge_list[source]=[target]

	def writeToFile(self):
		output_file = self.output_folder + '/prediction_edge_list_' + str(self.interval_id)
		out_f = open(output_file,'w')
		for source in self.edge_list:
			out_f.write(source + customListToString(self.edge_list[source]) + '\n')
		out_f.close()

def customListToString(mylist):
	output = ''
	for item in mylist:
		output  += (' ' + str(item))
	return output		

if __name__=='__main__':
	if len(sys.argv)==3:
		prediction_f = open(sys.argv[1],'r')
		output_folder = sys.argv[2]
		if not os.path.isdir(output_folder):
			os.mkdir(output_folder, 0777)
	
		iv_id = 1
		iel = IntervalEdgeList(iv_id,output_folder)

		for line in prediction_f:
			splitted_line = line.split()
			if iv_id != int(splitted_line[0]):
				iel.writeToFile()
				iel.clear()
				iv_id += 1
				iel.setIntervalId(iv_id)
			iel.addEdge(splitted_line[1],splitted_line[2])			
		iel.writeToFile()
		
		prediction_f.close()
		print 'Prediction file preprocessed.'
	else:
		print 'Usage: <prediction_file> <preprocessed_output_folder>' 
