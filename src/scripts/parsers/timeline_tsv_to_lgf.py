#!/usr/bin/python

import sys

class Identity:
	def getOrigiTime(self, num):
		return num

def parseTsvToLgf(convert_obj, tsv_file_path, lgf_file_path, source_index, target_index, time_index):
	source_index=int(source_index)
	target_index=int(target_index)
	time_index=int(time_index)

	tsv_file = open(tsv_file_path, 'r' )
	vertexIds={}
	print 'Storing different vertices is in progress..'
	for line in tsv_file:	
		split_line = line.split()	
		vertexIds[split_line[source_index]]=1
		if split_line[target_index]!='None' and int(split_line[target_index]) != -1:
			vertexIds[split_line[target_index]]=1
	tsv_file.close()

	lgf_file= open(lgf_file_path, 'w' )
	tsv_file_read_again = open(tsv_file_path, 'r' )
	lgf_file.write('@nodes' + '\n')
	lgf_file.write('label' + '\n')
	print 'Writing vertices to lgf file is in progress..'
	for nodeId in vertexIds.keys():
		lgf_file.write(nodeId + '\n')


	lgf_file.write('@arcs' + '\n')
	lgf_file.write('labels time' + '\n')
	print 'Writing edges to lgf file is in progress..'
	counter=0
	for edge in tsv_file_read_again:
		split_line = edge.split()
		if split_line[target_index]!='None' and int(split_line[target_index]) != -1:
			edge_line = split_line[source_index] + ' ' + split_line[target_index] + ' ' + str(counter) + ' ' + convert_obj.getOrigiTime(split_line[time_index]) + '\n'
			lgf_file.write(edge_line)
			counter += 1	
	
	tsv_file_read_again.close()
	lgf_file.close()
	print '\n' + 'Number of vertices:'
	print len(vertexIds)
	print 'Number of edges:'
	print str(counter)
	print '\n' + 'lgf format was created from tsv.'

if __name__=="__main__":
	if len(sys.argv)==6:
		identity = Identity()
		parseTsvToLgf(identity, sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],sys.argv[5])
	else:
		print 'Usage: <tsv_file_path> <lgf_file_path> <source_index_in_tsv> <target_index_in_tsv> <time_index_in_tsv>'
		print 'HINT:'
		print 'For Robi\' first movement: <tsv_file_path> <lgf_file_path> 1 2 0'
		print 'For Robi\'s postfix movement: <tsv_file_path> <lgf_file_path> 1 2 0'
