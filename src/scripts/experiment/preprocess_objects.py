import os, json, subprocess
import config_errors as ces

dir_append = '/src/scripts/parsers/'

class PreprocessBase(object):
	def __init__(self, own_config, global_config):
		self.name = own_config["name"]
		self.repo_path = ""
		self.input_path = ""
		self.input_type = ""
		self.task_folder = ""
		self.config = own_config

	def selfAttributeTest(self, own_config, global_config):
		if not "repo_dir" in global_config:
			raise ces.RepoPathError()
		if not "input" in own_config:
			raise ces.InputError(self.name)
		if not "task" in own_config:
			raise ces.TaskError(self.name)

	def selfTest(self):
		if self.input_path == "":
			raise ces.InputNameNotFoundError(self.config["input"])
		if self.task_folder == "":
			raise ces.TaskNameNotFoundError(self.config["task"])
		if not self.input_type == "toplist":	
			raise ces.InputCompatibilityError(self.name, self.input_type, ['toplist']) 

	def loadConfig(self, own_config, global_config):
		self.name = own_config["name"]
		self.repo_path = global_config["repo_dir"]
		for input_item in global_config["inputs"]:
			if input_item["name"] == own_config["input"]:
				self.input_type = input_item["type"]
				self.input_path = input_item["path"]
		for task in global_config["tasks"]:
			if task["name"] == own_config["task"]:
				self.task_folder =  task["path"]
				break

class ToplistToEdgeList(PreprocessBase):
	def __init__(self, own_config, global_config):
		super(ToplistToEdgeList, self).__init__(own_config, global_config)
		super(ToplistToEdgeList, self).selfAttributeTest(own_config, global_config)
		super(ToplistToEdgeList, self).loadConfig(own_config, global_config)	

	def run(self):
		executed = False
		try:
			super(ToplistToEdgeList, self).selfTest()
			output_folder = self.task_folder + '/data/preprocessed'
			if not os.path.exists(output_folder):
				os.makedirs(output_folder)
			script_path = self.repo_path + dir_append + 'prediction_to_edge_list.py'
			subprocess.check_call([script_path, self.input_path, output_folder])
			executed = True
		except ces.ErrorWithMsg as myerr:
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed


class ToplistToLgf(PreprocessBase):
	def __init__(self, own_config, global_config):
		super(ToplistToLgf, self).__init__(own_config, global_config)
		self.origi_task = ""
		self.origi_json_output = ""
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)	

	def selfTest(self):
		super(ToplistToLgf, self).selfTest()
		if self.origi_task == "":
			raise ces.OriginalTaskNameNotFoundError(self.config["original_task"])
		if not os.path.exists(self.origi_json_output):
			raise ces.TaskLemonIntervalsJsonError(self.name, self.origi_task)
	
	def selfAttributeTest(self, own_config, global_config):
		super(ToplistToLgf, self).selfAttributeTest(own_config, global_config)
		if not "original_task" in own_config:
			raise ces.OriginalTaskError(self.name)

	def loadConfig(self, own_config, global_config):
		super(ToplistToLgf, self).loadConfig(own_config, global_config)
		for task in global_config["tasks"]:
			if task["name"] == own_config["original_task"]:
				self.origi_task = task["path"]
				self.origi_json_output = task["path"] + '/lemon_output/intervals.json'

	def run(self):
		executed = False
		try:
			self.selfTest()
			output_folder = self.task_folder + '/data'
			if not os.path.exists(output_folder):
				os.makedirs(output_folder)
			script_path = self.repo_path + dir_append + 'timeline_prediction_to_lgf.py'
			print self.origi_json_output
			print script_path
			subprocess.check_call([script_path, self.input_path, output_folder + '/predicted_timeline.lgf', self.origi_json_output])
			executed = True
		except ces.ErrorWithMsg as myerr:
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed

