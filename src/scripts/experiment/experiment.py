#!/usr/bin/python

import sys,json,os
import preprocess_objects as pos
import lemon_objects as los
import visualization_objects as vos
import postprocess_objects as ppos
import config_errors as ces

# available options for configuration
input_types = ['lgf', 'toplist']
preprocess_job_types = ['toplist_to_lgf', 'toplist_to_edgelist']
lemon_job_types = ['original_centrality', 'predicted_centrality', 'predicted_centrality_with_lookback']
visualization_job_types = ['html_tables', 'graph_stat', 'prev_interval', 'centrality_eval', 'link_prediction_eval']
postprocess_job_types = ['edges_to_toplist']

def executeConfig(config):
	try:
		for input_item in config["inputs"]:
			if not "name" in input_item:
				raise ces.MissingNameError('input')
			if not "type" in input_item:
				raise ces.MissingTypeError('input')
			if not os.path.exists(input_item["path"]):
				input_exists = False
				raise ces.InputPathError(input_item["name"], input_item["path"])
			if not input_item["type"] in input_types: 
				input_exists = False
				raise ces.InputTypeError(input_item["type"], input_types)
		input_exists = True

		if(input_exists):
			for task_item in config["tasks"]:
				if not "name" in task_item:
					raise ces.MissingNameError('task')
				if not os.path.exists(task_item["path"]):
					os.makedirs(task_item["path"])	

		if(input_exists):
			preprocess_executed = True
			if ("preprocess_jobs" in config) and (len(config["preprocess_jobs"]) > 0):
				preproc_list = []
				for job_config in config["preprocess_jobs"]:
					if not "name" in job_config:
						raise ces.MissingNameError('preprocess')
					if not "type" in job_config:
						raise ces.MissingTypeError('preprocess')
				
					if job_config["type"] == "toplist_to_edgelist":
						preproc_list.append(pos.ToplistToEdgeList(job_config, config))
					elif job_config["type"] == "toplist_to_lgf":
						preproc_list.append(pos.ToplistToLgf(job_config, config))
					else:
						raise ces.PreprocessTypeError(job_config['type'], preprocess_job_types)
				for job in preproc_list:	
					preprocess_executed = preprocess_executed and job.run()
		else:
			raise RuntimeError('Errors occured in "tasks" configuration!')

		if(preprocess_executed):
			lemon_executed = True	
			if ("lemon_jobs" in config) and (len(config["lemon_jobs"]) > 0):
				lemon_list = []
				for job_config in config["lemon_jobs"]:
					if not "name" in job_config:
						raise ces.MissingNameError('lemon')
					if not "type" in job_config:
						raise ces.MissingTypeError('lemon')

					if job_config["type"] == "original_centrality":
						lemon_list.append(los.OriginalCentrality(job_config,config))
					elif job_config["type"] == "predicted_centrality":
						lemon_list.append(los.PredictedCentrality(job_config,config))
					elif job_config["type"] == "predicted_centrality_with_lookback":
						lemon_list.append(los.PredictedCentralityWithLookback(job_config,config))
					else:
						raise ces.LemonTypeError(job_config['type'], lemon_job_types)
				for job in lemon_list:
					lemon_executed = lemon_executed and job.run()	
		else:
			raise RuntimeError('Errors occured in "preprocess_jobs" configuration!')
	
		if(lemon_executed):
			postprocess_executed = True
			if ("postprocess_jobs" in config) and (len(config["postprocess_jobs"]) > 0):
				postproc_list = []
				for job_config in config["postprocess_jobs"]:
					if not "name" in job_config:
						raise ces.MissingNameError('postprocess')
					if not "type" in job_config:
						raise ces.MissingTypeError('postprocess')
				
					if job_config["type"] == "edges_to_toplist":
						postproc_list.append(ppos.EdgesToToplist(job_config, config))
					else:
						raise ces.PostprocessTypeError(job_config['type'], postprocess_job_types)
				for job in postproc_list:	
					postprocess_executed = postprocess_executed and job.run()
		else:
			raise RuntimeError('Errors occured in "lemon_jobs" configuration!')

		if(postprocess_executed):
			visualization_executed = True
			if ("visualization_jobs" in config) and (len(config["visualization_jobs"]) > 0):
				visu_list = []
				for job_config in config["visualization_jobs"]:
					if not "name" in job_config:
						raise ces.MissingNameError('visualization')
					if not "type" in job_config:
						raise ces.MissingTypeError('visualization')
				
					if job_config["type"] == "html_tables":
						visu_list.append(vos.HtmlTables(job_config, config))
					elif job_config["type"] == "graph_stat":
						visu_list.append(vos.GraphStat(job_config, config))
					elif job_config["type"] == "prev_interval":
						visu_list.append(vos.PrevInterval(job_config, config))		
					elif job_config["type"] == "centrality_eval":
						visu_list.append(vos.CentralityEval(job_config, config))
					elif job_config["type"] == "link_prediction_eval":
						visu_list.append(vos.LinkPredictionEval(job_config, config))
					else:
						raise ces.VisualizationTypeError(job_config['type'], visualization_job_types)
				for job in visu_list:
					visualization_executed = visualization_executed and job.run()	
		else:
			raise RuntimeError('Errors occured in "postprocess_jobs" configuration!')

		if not visualization_executed:
			raise RuntimeError('Errors occured in "visualization_jobs" configuration!')
	except ces.ErrorWithMsg as myerr:
		print myerr.msg
	except RuntimeError as rte:
		print rte.args[0]


if __name__ == '__main__':
	if len(sys.argv) == 2:
		json_f = open(sys.argv[1],'r')
		config = json.load(json_f)
		json_f.close()
		executeConfig(config)
	else:
		print 'Usage: <json_config_file>'
