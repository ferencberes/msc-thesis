#!/usr/bin/python

import sys, json
from jinja2 import Template

# it will be the width of a measure column in pixel
column_width = 120

html_tmpl = Template(u'''\
<!DOCTYPE html>
<html>
	<head>
{{gen_head}}
	</head>
	<body>
{{gen_body}}
	</body>
</html>''')


def generateHead():
	header_tmpl = Template(u'''\
		<title>Temporal centrality test</title>
		<style>	 
			table.interval_table {
				border-collapse: collapse;
				border: 3px solid black;
			}
			table.measure_table {
				border-collapse: separate;
				border-spacing: 0px;
			}
			th.measure_name {
				font-family: "Times New Roman", Times, serif;
				background-color: #7CA4EB;
				font-size: 15pt;
			}
			th.record_header {
				font-family: "Times New Roman", Times, serif;
				background-color: #B1C9F2;
				font-size: 10pt;
			}
			td {
				font-family: Arial, Helvetica, sans-serif;
				font-size: 10pt;
				font-style: normal;
			}
			td.id_column {
				width:{{col_size-30}}px;
			}
			td.delta_column {
				width:30px;
			}
   		</style>''')
	return header_tmpl.render(col_size=column_width)

def getMeasure(measureSet):
	measures_tmpl = Template(u'''\
			<tr>
			{%- for measure in measures %}
				<th class="measure_name">{{measure}}</th>
			{%- endfor %}
			</tr>''')
	return measures_tmpl.render(measures=measureSet)	

# this function prints one record with its value and color into the html table
def getRecordWithColor(record_id,record_delta):
	record_tmpl =  Template(u'''\
						<tr>
							<td class="id_column">{{identifier}}</td>
							<td class="delta_column", BGCOLOR="{{colorChooser(delta)}}">{{delta}}</td>
						</tr>''')
	return record_tmpl.render(colorChooser=colorChooser ,identifier=record_id, delta=record_delta)

# chooses color according to delta
def colorChooser(delta):
	green="#32CD32"
	red="#FF6347"
	grey="#C0C0C0"
	orange="#FFA500"
	
	if delta == "new":
		return orange
	if delta == 0:
		return grey
	if delta > 0:
		return green
	if delta < 0:
		return red

# writes one interval into the html table
def writeIntervalToHtml(previous, current, measureSet):
	interval_tmpl = Template('''\
		<table class="interval_table" border="1">
{{getMeasure(measures)}}
			<tr>
{{measure_with_records}}
			</tr>
		</table>''')

	measure_table_tmpl = Template('''\
				<th>
					<table class="measure_table" border="1">
						<tr>
							<th class="record_header">id</th>
							<th class="record_header">delta</th>
						</tr>
{{records}}
					</table>
				</th>''')

	m_with_r = ''
	for measure in measureSet:
		recs = ''
		resultMap = current[measure]
		for pos in resultMap:
			identifier=resultMap[pos]
			# write a record to the html table
			recs += (getRecordWithColor(identifier,computeDelta(previous,measure,identifier,pos)) + '\n')
		m_with_r += (measure_table_tmpl.render(records=recs) + '\n')

	return interval_tmpl.render(getMeasure=getMeasure, measures=measureSet, measure_with_records=m_with_r)

# computes the change from the previous and current position
def computeDelta(previous,measure,identifier,currentPos):
	output = "new"	
	if previous:	
		measureMap=previous[measure]
		delta=0
		presentInPrevious=False
		for pos in measureMap:
			if measureMap[pos]==identifier:
				delta=pos-currentPos
				presentInPrevious=True
				break
		if presentInPrevious:
			output=delta	
	return output

###main###
if len(sys.argv) != 3 :
	print 'Usage: <centrality_json_log_path> <output_file_path>'
	#print len(sys.argv)
else:
	json_data=open(sys.argv[1])

	data = json.load(json_data)
	json_data.close()

	measureSet = data["centrality_test"]["measures"]
	topKNumber = data["centrality_test"]["topK"]
	
	f = open(sys.argv[2],'w')
	generated_body = ''
	previous={}
	for interval in data["centrality_test"]["intervals"]:
		current={}
		for measure in measureSet:
			measureMap={}	
			valueMap={}	
			for record in interval["interval"]["measures"][measure]:
				# here we gives the id and value data
				measureMap[record["pos"]]=record["id"]
				valueMap[record["pos"]]=record["value"]
			current[measure]=measureMap
			# TODO: valueMap should be packed inside current!!!
		generated_body += (writeIntervalToHtml(previous,current,measureSet) + '\n')
		previous=current	

	gen_html = html_tmpl.render(gen_head=generateHead(), gen_body=generated_body)
	f.write(gen_html)
	f.close()

	print "The html table was created."
