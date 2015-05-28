import os, json, subprocess
import config_errors as ces

visu_dir_append = '/src/scripts/visualization/'
common_dir_append = '/src/scripts/common/'

class VisualizationBase(object):
	def __init__(self, own_config, global_config):
		self.name = own_config["name"]
		self.repo_path = "" 
		self.tasks = {}
		self.task_order = []
		self.config = own_config

	def selfAttributeTest(self, own_config, global_config):
		if not "repo_dir" in global_config:
			raise ces.RepoPathError()
		if not "tasks" in own_config:
			raise ces.VisualizationInputError(self.name, 'tasks')

	def selfTest(self):
		for task in self.tasks:
			if self.tasks[task] == "":
				raise ces.TaskNameNotFoundError(task)
			if not os.path.exists(self.tasks[task] + '/lemon_output'):
				raise ces.TaskLemonError(self.tasks[task] + '/lemon_output')
			if not os.path.exists(self.tasks[task] + '/lemon_output/intervals.json'):
				raise ces.TaskLemonIntervalsJsonError(self.tasks[task] + '/lemon_output')
	
	def loadConfig(self, own_config, global_config):
		self.repo_path = global_config["repo_dir"]
		self.task_order = []
		for task_item in own_config["tasks"]:
			self.task_order.append(task_item)
			item_found = False
			for task in global_config["tasks"]:
				if task["name"] == task_item:
					self.tasks[task_item] = task["path"]	
					item_found = True
					break
			if not item_found:
				self.tasks[task_item] = ""	
		
class HtmlTables(VisualizationBase):
	def __init__(self, own_config, global_config):
		super(HtmlTables, self).__init__(own_config, global_config)
		super(HtmlTables, self).selfAttributeTest(own_config, global_config)
		super(HtmlTables, self).loadConfig(own_config, global_config)

	def run(self):
		executed = False
		try:
			super(HtmlTables, self).selfTest()
			script_path = self.repo_path + visu_dir_append + 'topK_json_to_html.py'
			for task in self.tasks:
				json_file = self.tasks[task] + '/lemon_output/intervals.json'
				plot_dir = self.tasks[task] + '/plots'
				if not os.path.exists(plot_dir):
					os.makedirs(plot_dir)
				subprocess.check_call([script_path, json_file, plot_dir + '/top_k_tables.html'])
			executed = True
		except ces.ErrorWithMsg as myerr:	
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed
	
class GraphStat(VisualizationBase):
	def __init__(self, own_config, global_config):
		super(GraphStat, self).__init__(own_config, global_config)
		super(GraphStat, self).selfAttributeTest(own_config, global_config)
		super(GraphStat, self).loadConfig(own_config, global_config)

	def run(self):
		executed = False
		try:
			super(GraphStat, self).selfTest()
			script_path = self.repo_path + visu_dir_append + 'graph_stat_plots.py'
			for task in self.tasks:
				json_file = self.tasks[task] + '/lemon_output/intervals.json'
				plot_dir = self.tasks[task] + '/plots'
				if not os.path.exists(plot_dir):
					os.makedirs(plot_dir)
				subprocess.check_call([script_path, json_file, plot_dir])
			executed = True
		except ces.ErrorWithMsg as myerr:	
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed
	
class PrevInterval(VisualizationBase):
	def __init__(self, own_config, global_config):
		super(PrevInterval, self).__init__(own_config, global_config)
		self.metrics = [] 
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)

	def selfAttributeTest(self, own_config, global_config):
		super(PrevInterval, self).selfAttributeTest(own_config, global_config)
		if "metrics" not in own_config:
			raise ces.VisualizationMetricError(self.name)
	def selfTest(self):
		super(PrevInterval, self).selfTest()
		if len(self.metrics) == 0:
			raise ces.VisualizationEmptyMetricError(self.name)

	def loadConfig(self, own_config, global_config):
		super(PrevInterval, self).loadConfig(own_config, global_config)
		self.metrics = own_config["metrics"]

	def run(self):
		executed = False
		try:
			self.selfTest()
			script_path = self.repo_path + common_dir_append + 'eval_centrality_prediction.py'
			for task in self.tasks:
				json_file = self.tasks[task] + '/lemon_output/intervals.json'
				plot_dir = self.tasks[task] + '/plots'
				if not os.path.exists(plot_dir):
					os.makedirs(plot_dir)
				for metric in self.metrics:
					subprocess.check_call([script_path, plot_dir, metric, json_file])
			executed = True
		except ces.ErrorWithMsg as myerr:	
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed
	
class CentralityEval(PrevInterval):
	def __init__(self, own_config, global_config):
		super(CentralityEval, self).__init__(own_config, global_config)
		self.origi_task = ""
		self.plot_dir = ""
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)

	def selfAttributeTest(self, own_config, global_config):
		super(CentralityEval, self).selfAttributeTest(own_config, global_config)
		if "output_folder" not in own_config:
			raise ces.MissingOutputFolderError(self.name)
		if "original_task" not in own_config:
			raise ces.OriginalTaskError(self.name)

	def selfTest(self):
		#print self.tasks
		super(CentralityEval, self).selfTest()
		if self.origi_task == "":
			raise ces.OriginalTaskNameNotFoundError(self.name)		
		if not os.path.exists(self.origi_task + '/lemon_output'):
			raise ces.TaskLemonError(self.origi_task + '/lemon_output')
		if not os.path.exists(self.origi_task + '/lemon_output/intervals.json'):
			raise ces.TaskLemonIntervalsJsonError(self.origi_task + '/lemon_output')
	
	def loadConfig(self, own_config, global_config):
		super(CentralityEval, self).loadConfig(own_config, global_config)
		for task in global_config["tasks"]:
			if task["name"] == own_config["original_task"]:
				self.origi_task = task["path"]
				break
		self.plot_dir = own_config["output_folder"]

	def run(self):
		executed = False
		try:
			self.selfTest()
			script_path = self.repo_path + common_dir_append + 'eval_centrality_prediction.py'
			if not os.path.exists(self.plot_dir):
				os.makedirs(self.plot_dir)
			for metric in self.metrics:
				execution_params = [script_path, self.plot_dir, metric, self.origi_task + '/lemon_output/intervals.json']
				for task in self.task_order:
					execution_params.append(self.tasks[task] + '/lemon_output/intervals.json')
					execution_params.append(task)
				subprocess.check_call(execution_params)
			executed = True
		except ces.ErrorWithMsg as myerr:	
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed
	
class LinkPredictionEval:
	def __init__(self, own_config, global_config):
		self.name = own_config["name"]
		self.repo_path = "" 
		self.toplists = {}
		self.toplist_order = []
		self.metrics = [] 
		self.origi_task = ""
		self.plot_dir = ""
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)

	def selfAttributeTest(self, own_config, global_config):
		if not "repo_dir" in global_config:
			raise ces.RepoPathError()
		if not "inputs" in own_config:
			raise ces.VisualizationInputError(self.name, 'inputs')
		if "metrics" not in own_config:
			raise ces.VisualizationMetricError(self.name)
		if "output_folder" not in own_config:
			raise ces.MissingOutputFolderError(self.name)
		if "original_task" not in own_config:
			raise ces.OriginalTaskError(self.name)

	def selfTest(self):
		for toplist in self.toplists:
			if self.toplists[toplist] == "":
				raise ces.InputNameNotFoundError(toplist)
		if self.origi_task == "":
			raise ces.OriginalTaskNameNotFoundError(self.name)		
		if not os.path.exists(self.origi_task + '/lemon_output/edges_in_intervals'):
			raise ces.TaskLemonError(self.origi_task + '/lemon_output/edges_in_intervals')

	def loadConfig(self, own_config, global_config):
		self.repo_path = global_config["repo_dir"]
		self.toplist_order = []
		for input_item in own_config["inputs"]:
			item_found = False
			self.toplist_order.append(input_item)
			for orig_input_item in global_config["inputs"]:
				if orig_input_item["name"] == input_item:
					if orig_input_item["type"] != "toplist":
						raise ces.InputCompatibilityError(self.name, orig_input_item["type"], ['toplist'])	
					else:
						self.toplists[input_item] = orig_input_item["path"]	
						item_found = True
						break
			if not item_found:
				self.toplists[input_item] = ""	

		self.metrics = own_config["metrics"]
		for task in global_config["tasks"]:
			if task["name"] == own_config["original_task"]:
				self.origi_task = task["path"]
				break
		self.plot_dir = own_config["output_folder"]

	def run(self):
		executed = False
		try:
			self.selfTest()
			script_path = self.repo_path + common_dir_append + 'eval_edge_prediction.py'
			json_file = self.origi_task + '/lemon_output/intervals.json'
			interval_edges_folder = self.origi_task + '/lemon_output/edges_in_intervals'
			if not os.path.exists(self.plot_dir):
				os.makedirs(self.plot_dir)
			for metric in self.metrics:
				execution_params = [script_path, self.plot_dir, metric, json_file, interval_edges_folder]
				for toplist in self.toplist_order:
					execution_params.append(self.toplists[toplist])
					execution_params.append(toplist)
				subprocess.check_call(execution_params)
			executed = True
		except ces.ErrorWithMsg as myerr:	
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed
	

