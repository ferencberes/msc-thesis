class ErrorWithMsg(Exception):
	def __init__(self):
		self.msg = ''

class RepoPathError(ErrorWithMsg):
	def __init__(self):
		self.msg = 'RepoPathError: There was no path set for "repo_dir"!'

class MissingNameError(ErrorWithMsg):
	def __init__(self, object_type):
		self.msg = 'MissingNameError: There was no "name" set for an "' + object_type +'" object!'

class MissingTypeError(ErrorWithMsg):
	def __init__(self, object_type):
		self.msg = 'MissingTypeError: There was no "type" set for an "' + object_type +'" object!'


# Errors related to inputs
class InputError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'InputError: There was no "input" set for "' + object_name +'" object!'

class InputNameNotFoundError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'InputNameNotFoundError: There was no input object found with name "' + object_name +'"!'

class InputPathError(ErrorWithMsg):
	def __init__(self, object_name, path):
		self.msg = 'InputPathError: The path ' + path + ' set for "' + object_name +'" object does not exist!'

class InputTypeError(ErrorWithMsg):
	def __init__(self, current_type, enabled_type_list):
		self.msg = 'InputTypeError: There is no input type "' + current_type +'"! Choose a type from ' + str(enabled_type_list)

class InputCompatibilityError(ErrorWithMsg):
	def __init__(self, object_name, current_type, enabled_type_list):
		self.msg = 'InputCompatibilityError: Input type "' + current_type +'" is not compatible with "' + object_name+ '" object! Choose a type from ' + str(enabled_type_list)

# Errors related to tasks 
class TaskError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'TaskError: There was no "task" set for "' + object_name +'" object!'

class TaskNameNotFoundError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'TaskNameNotFoundError: There was no task object found with name "' + object_name +'"!'

# finally non existing task paths are created! This error is never raised.
class TaskPathError(ErrorWithMsg):
	def __init__(self, object_name, path):
		self.msg = 'TaskPathError: The path ' + path + ' set for "' + object_name +'" object does not exist!'

# Tenyleg belevenni a kodba:  ahol origi_task-rol beszelunk...
class TaskLemonError(ErrorWithMsg):
	def __init__(self, object_name, path):
		self.msg = 'TaskLemonError: The path ' + path + '/lemon_output does not exist for "' + object_name +'" object!'

class TaskLemonIntervalsJsonError(ErrorWithMsg):
	def __init__(self, object_name, path):
		self.msg = 'TaskLemonIntervalsJsonError: The path ' + path + '/lemon_output/intervals.json does not exist for "' + object_name +'" object!'

class OriginalTaskError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'OriginalTaskError: There was no original task set for "' + object_name +'" object!'

class OriginalTaskNameNotFoundError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'OriginalTaskNameNotFoundError: There was no task object found with name "' + object_name +'"!'

# Errors related to preprocess jobs
class PreprocessError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'PreprocessError: There was no "preprocess" set for "' + object_name +'" object!'

class PreprocessNameNotFoundError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'PreprocessNameNotFoundError: There was no preprocess object found with name "' + object_name +'"!'

class PreprocessTypeError(ErrorWithMsg):
	def __init__(self, current_type, enabled_type_list):
		self.msg = 'PreprocessTypeError: There is no preprocess type "' + current_type +'"! Choose a type from ' + str(enabled_type_list)

class PreprocessCompatibilityError(ErrorWithMsg):
	def __init__(self, current_type, enabled_type_list):
		self.msg = 'PreprocessCompatibilityError: Preprocess type "' + current_type +'" is not compatible with ' + object_name+ '" object"! Choose a type from ' + str(enabled_type_list)

# Errors related to lemon jobs
class LemonTypeError(ErrorWithMsg):
	def __init__(self, current_type, enabled_type_list):
		self.msg = 'LemonTypeError: There is no lemon job type "' + current_type +'"! Choose a type from ' + str(enabled_type_list)

class MissingParameterError(ErrorWithMsg):
	def __init__(self, object_name, parameter_name):
		self.msg = 'MissingParameterError: set parameter "' + parameter_name + '" for "' + object_name + '" lemon object!'

# Errors related to postprocess jobs
class PostprocessTypeError(ErrorWithMsg):
	def __init__(self, current_type, enabled_type_list):
		self.msg = 'PostprocessTypeError: There is no postprocess type "' + current_type +'"! Choose a type from ' + str(enabled_type_list)

class PostprocessEdgeTypesError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'PostprocessEdgeTypesError: There was no "edge_types" list set for "' + object_name +'" object!'

class PostprocessEmptyEdgeTypesError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'PostprocessEmptyEdgeTypesError: The "edge_types" list is empty for "' + object_name +'" object! Choose from "homophily" or "all" options!'

# Errors related to visualization jobs
class VisualizationInputError(ErrorWithMsg):
	def __init__(self, object_name, append):
		self.msg = 'VisualizationInputError: Enumerate chosen ' + append + ' under "'+ append + '" key for "' + object_name +'" object!'

class VisualizationTypeError(ErrorWithMsg):
	def __init__(self, current_type, enabled_type_list):
		self.msg = 'VisualizationTypeError: There is no visualization job type "' + current_type +'"! Choose a type from ' + str(enabled_type_list)

class VisualizationMetricError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'VisualizationMetricError: There was no evaluator metric set for "' + object_name +'" object!'

class VisualizationEmptyMetricError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'VisualizationEmptyMetricError: There was no evaluator metric set for "' + object_name +'" object! Choose from "ndcg","nrbm" and "precision"!'

class MissingOutputFolderError(ErrorWithMsg):
	def __init__(self, object_name):
		self.msg = 'MissingOutputFolderError: There was no "output_folder" set for "' + object_name +'" visualization object!'

