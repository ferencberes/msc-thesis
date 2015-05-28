import os, json, subprocess
import config_errors as ces

dir_append = '/src/scripts/experiment/'

class LemonBase(object):
	def __init__(self, own_config, global_config):
		self.name = own_config["name"]
		self.repo_path = ""
		self.input_path = ""
		self.task_folder = ""
		self.start_time = ""
		self.delta = ""
		self.centr_lookback_c = ""
		self.combine_factor = ""
		self.graph_lookback_c = ""
		self.enable_multi_e = ""
		self.max_num_step = ""
		self.top_k = ""
		self.measures = ""
		self.config = own_config

	def selfAttributeTest(self, own_config, global_config):
		if not "repo_dir" in global_config:
			raise ces.RepoPathError()
		if not "task" in own_config:
			raise ces.TaskError(self.name)
		if not "start_time" in own_config:
			raise ces.MissingParameterError(self.name, "start_time")
		if not "delta" in own_config:
			raise ces.MissingParameterError(self.name, "delta")
		if not "centrality_prev_interval_count" in own_config:
			raise ces.MissingParameterError(self.name, "centrality_prev_interval_count")
		if not "combine_factor" in own_config:
			raise ces.MissingParameterError(self.name, "combine_factor")
		if not "graph_stat_prev_interval_count" in own_config:
			raise ces.MissingParameterError(self.name, "graph_stat_prev_interval_count")
		if not "enable_multi_edges" in own_config:
			raise ces.MissingParameterError(self.name, "enable_multi_edges")
		if not "max_num_of_steps" in own_config:
			raise ces.MissingParameterError(self.name, "max_num_of_steps")
		if not "top_k" in own_config:
			raise ces.MissingParameterError(self.name, "top_k")
		if not "measures" in own_config:
			raise ces.MissingParameterError(self.name, "measures")

	def loadConfig(self, own_config, global_config):		
		self.repo_path = global_config["repo_dir"]
		self.start_time = str(own_config["start_time"])
		self.delta = str(own_config["delta"])
		self.centr_lookback_c = str(own_config["centrality_prev_interval_count"])
		self.combine_factor = str(own_config["combine_factor"])
		self.graph_lookback_c = str(own_config["graph_stat_prev_interval_count"])
		self.enable_multi_e = str(own_config["enable_multi_edges"])
		self.max_num_step = str(own_config["max_num_of_steps"])
		self.top_k = str(own_config["top_k"])
		self.measures = own_config["measures"]
		for task in global_config["tasks"]:
			if task["name"] == own_config["task"]:
				self.task_folder = task["path"]
				break
		
	def selfTest(self):
		if self.task_folder == "":
			raise ces.TaskNameNotFoundError(self.config["task"])

class OriginalCentrality(LemonBase):
	def __init__(self, own_config, global_config):
		super(OriginalCentrality, self).__init__(own_config, global_config)
		self.input_type = ""
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)	

	def selfAttributeTest(self, own_config, global_config):
		super(OriginalCentrality, self).selfAttributeTest(own_config, global_config)
		if not "input" in own_config:
			raise ces.InputError(self.name)

	def selfTest(self):
		super(OriginalCentrality, self).selfTest()
		if self.input_path == "":
			raise ces.InputNameNotFoundError(self.config["input"])
		if self.input_type != "lgf":
			raise ces.InputCompatibilityError(self.name, self.input_type, ["lgf"])

	def loadConfig(self, own_config, global_config):
		super(OriginalCentrality, self).loadConfig(own_config, global_config)
		for input_item in global_config["inputs"]:
			if input_item["name"] == own_config["input"]:
				self.input_path = input_item["path"]
				self.input_type = input_item["type"]
				break

	def run(self):
		executed = False
		try:
			self.selfTest()
			output_folder = self.task_folder + '/lemon_output'
			script_path = self.repo_path + dir_append + 'runTemporalLemonTest.sh'
			execute_params = [script_path, self.input_path, output_folder, self.start_time, self.delta, self.centr_lookback_c, self.combine_factor, self.graph_lookback_c, self.enable_multi_e, self.max_num_step, self.top_k]
			for measure in self.measures:
				execute_params.append('-' + measure)
			subprocess.check_call(execute_params)
			executed = True
		except ces.ErrorWithMsg as myerr:
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed

class PredictedCentrality(LemonBase):
	def __init__(self, own_config, global_config):
		super(PredictedCentrality, self).__init__(own_config, global_config)
		self.preproc_type = ""	
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)	

	def selfAttributeTest(self, own_config, global_config):
		super(PredictedCentrality, self).selfAttributeTest(own_config, global_config)
		if not "preprocess" in own_config:
			raise ces.PreprocessError(self.name)

	def selfTest(self):
		super(PredictedCentrality, self).selfTest()
		if self.preproc_type == "":
			raise ces.PreprocessNameNotFoundError(self.config["preprocess"])
		if self.preproc_type != "toplist_to_lgf":
			raise ces.PreprocessCompatibilityError(self.name, ["toplist_to_lgf"])

	def loadConfig(self, own_config, global_config):
		super(PredictedCentrality, self).loadConfig(own_config, global_config)
		for task in global_config["tasks"]:
			if task["name"] == own_config["task"]:
				self.input_path = self.task_folder + '/data/predicted_timeline.lgf'
				break
		for preproc in global_config["preprocess_jobs"]:
			if preproc["name"] == own_config["preprocess"]:
				self.preproc_type = preproc["type"]
				break

	def run(self):
		executed = False
		try:
			self.selfTest()
			output_folder = self.task_folder + '/lemon_output'
			script_path = self.repo_path + dir_append + 'runTemporalLemonTest.sh'
			execute_params = [script_path, self.input_path, output_folder, self.start_time, self.delta, self.centr_lookback_c, self.combine_factor, self.graph_lookback_c, self.enable_multi_e, self.max_num_step, self.top_k]
			for measure in self.measures:
				execute_params.append('-' + measure)
			subprocess.check_call(execute_params)
			executed = True
		except ces.ErrorWithMsg as myerr:
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed

class PredictedCentralityWithLookback(LemonBase):
	def __init__(self, own_config, global_config):
		super(PredictedCentralityWithLookback, self).__init__(own_config, global_config)
		self.preproc_folder = ""
		self.preproc_type = ""	
		self.input_type = ""
		self.num_of_pred_edges = ""
		self.selfAttributeTest(own_config, global_config)
		self.loadConfig(own_config, global_config)	

	def selfAttributeTest(self, own_config, global_config):
		super(PredictedCentralityWithLookback, self).selfAttributeTest(own_config, global_config)
		if not "preprocess" in own_config:
			raise ces.PreprocessError(self.name)
		if not "input" in own_config:
			raise ces.InputError(self.name)
		if not "num_of_predicted_edges" in own_config:
			raise ces.MissingParameterError(self.name, "num_of_predicted_edges")

	def selfTest(self):
		super(PredictedCentralityWithLookback, self).selfTest()
		if self.input_path == "":
			raise ces.InputNameNotFoundError(self.config["input"])
		if self.input_type != "lgf":
			raise ces.InputCompatibilityError(self.name, self.input_type, ["lgf"])
		if self.preproc_type == "":
			raise ces.PreprocessNameNotFoundError(self.config["preprocess"])
		if self.preproc_type != "toplist_to_edgelist":
			raise ces.PreprocessCompatibilityError(self.name, ["toplist_to_edgelist"])

	def loadConfig(self, own_config, global_config):
		super(PredictedCentralityWithLookback, self).loadConfig(own_config, global_config)
		self.num_of_pred_edges = str(own_config["num_of_predicted_edges"])
		for input_item in global_config["inputs"]:
			if input_item["name"] == own_config["input"]:
				self.input_path = input_item["path"]
				self.input_type = input_item["type"]
				break
		for task in global_config["tasks"]:
			if task["name"] == own_config["task"]:
				self.preproc_folder = self.task_folder + '/data/preprocessed'
				break
		for preproc in global_config["preprocess_jobs"]:
			if preproc["name"] == own_config["preprocess"]:
				self.preproc_type = preproc["type"]
				break

	def run(self):
		executed = False
		try:
			self.selfTest()
			output_folder = self.task_folder + '/lemon_output'
			script_path = self.repo_path + dir_append + 'runTemporalLemonTestWithPrediction.sh'
			execute_params = [script_path, self.input_path, output_folder, self.preproc_folder, self.num_of_pred_edges, self.start_time, self.delta, self.centr_lookback_c, self.combine_factor, self.graph_lookback_c, self.enable_multi_e, self.max_num_step, self.top_k]
			for measure in self.measures:
				execute_params.append('-' + measure)
			subprocess.check_call(execute_params)
			executed = True
		except ces.ErrorWithMsg as myerr:
			print myerr.msg
		except Exception as ex:
			print ex.args[0]
		finally:
			return executed
