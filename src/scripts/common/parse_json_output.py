import json
import ranking_metrics as rm

def getMeasureNames(json_data):
	measures=[]
	for m_name in json_data["centrality_test"]["measures"]:
		measures.append(m_name)
	return measures

def getTopListOfInterval(measure_name_tag):
	toplist=[]
	for record in measure_name_tag:
		toplist.append(record["id"])
	return toplist

def loadMeasureRankings(measure_name_list, orig_json_data):
	orig_measure_toplists={}
	for m_name in measure_name_list:
		orig_measure_toplists[m_name]=[]

	for interval in orig_json_data["centrality_test"]["intervals"]:
		for m_name in measure_name_list:
			if m_name in interval["interval"]["measures"]:
				orig_measure_toplists[m_name].append(getTopListOfInterval(interval["interval"]["measures"][m_name]))
			else:
				orig_measure_toplists[m_name].append([])
	return orig_measure_toplists
	
def extractToplistsFromJson(orig_json_data):
	measure_name_list = getMeasureNames(orig_json_data)
	orig_measure_toplists = loadMeasureRankings(measure_name_list, orig_json_data)
	return [measure_name_list, orig_measure_toplists]	

