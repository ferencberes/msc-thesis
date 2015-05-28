import os, json, subprocess
import config_errors as ces

dir_append = '/src/scripts/parsers/'

class PostprocessBase(object):
	def __init__(self, own_config, global_config):
		self.name = own_config["name"]
		self.repo_path = ""
		self.input_path = ""
		self.task_folder = ""
		self.config = own_config

	def selfAttributeTest(self, own_config, global_config):
		if not "repo_dir" in global_config:
			raise ces.RepoPathError()
		if not "task" in own_config:
			raise ces.TaskError(self.name)

	def selfTest(self):
		if self.task_folder == "":
			raise ces.TaskNameNotFoundError(self.config["task"])

	def loadConfig(self, own_config, global_config):
		self.name = own_config["name"]
		self.repo_path = global_config["repo_dir"]
		for task in global_config["tasks"]:
			if task["name"] == own_config["task"]:
				self.task_folder =  task["path"]
				break

class EdgesToToplist(PostprocessBase):
	def __init__(self, own_config, global_config):
		super(EdgesToToplist, self).__init__(own_config, global_config)
		self.edge_types = []
		self.lemon_edges_folder = ""	
		self.lemon_json_file = ""
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)

	def selfAttributeTest(self, own_config, global_config):
		super(EdgesToToplist, self).selfAttributeTest(own_config, global_config)
		if not "edge_types" in own_config:
			raise ces.PostprocessEdgeTypesError(self.name)

	def loadConfig(self, own_config, global_config):
		super(EdgesToToplist, self).loadConfig(own_config, global_config)	
		self.edge_types = own_config["edge_types"]
		self.lemon_edges_folder = self.task_folder + '/lemon_output/edges_in_intervals' 
		self.lemon_json_file = self.task_folder + '/lemon_output/intervals.json'
	
	def selfTest(self):
		super(EdgesToToplist, self).selfTest()
		if len(self.edge_types) == 0:
			raise ces.PostprocessEmptyEdgeTypesError(self.name)
		if not os.path.exists(self.task_folder + '/lemon_output'):
			raise ces.TaskLemonError(self.task_folder + '/lemon_output')
		if not os.path.exists(self.task_folder + '/lemon_output/intervals.json'):
			raise ces.TaskLemonIntervalsJsonError(self.task_folder + '/lemon_output')

	def run(self):
		executed = False
		try:
			self.selfTest()
			output_folder = self.task_folder + '/data/toplists'
			if not os.path.exists(output_folder):
				os.makedirs(output_folder)
			script_path = self.repo_path + dir_append + 'interval_graph_data_to_toplist.py'
			json_f = open(self.lemon_json_file)
			json_data = json.load(json_f)
			start_interval = 0
			stop_interval = len(json_data["centrality_test"]["intervals"]) - 1 
			if 'homophily' in self.edge_types:
				subprocess.check_call([script_path, self.lemon_edges_folder, 'interval_homophily_edges', str(start_interval), str(stop_interval), output_folder + '/all_homophily_edges.gtl'])
			if 'all' in self.edge_types:
				subprocess.check_call([script_path, self.lemon_edges_folder, 'interval_edges', str(start_interval), str(stop_interval), output_folder + '/all_edges.gtl'])
			executed = True
		except ces.ErrorWithMsg as myerr:
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed


