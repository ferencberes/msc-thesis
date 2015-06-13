import sys

tsv_file = open(sys.argv[1], 'r' )

vertexIds={}
print 'Storing different vertices is in progress..'
for line in tsv_file:	
	split_line = line.split()	
	vertexIds[split_line[0]]=1
	vertexIds[split_line[1]]=1
tsv_file.close()

lgf_file= open(sys.argv[2], 'w' )
tsv_file_again = open(sys.argv[1], 'r' )

lgf_file.write('@nodes' + '\n')
#lgf_file.write('label id' + '\n')
lgf_file.write('label' + '\n')

print 'Writing vertices to lgf file is in progress..'
for nodeId in vertexIds.keys():
#	lgf_file.write(nodeId + ' ' + nodeId + '\n')
	lgf_file.write(nodeId + '\n')


lgf_file.write('@arcs' + '\n')
lgf_file.write('  labels' + '\n')

print 'Writing edges to lgf file is in progress..'
counter=0
for edge in tsv_file_again:
	split_line = edge.split()
	edge_line = split_line[0] + ' ' + split_line[1] + ' ' + str(counter) + '\n'
	lgf_file.write(edge_line)
	counter += 1	

tsv_file_again.close()
lgf_file.close()

print '\n' + 'Number of vertices:'
print len(vertexIds)

print 'Number of edges:'
print str(counter)

print '\n' + 'lgf format was created from tsv.'


