import ranking_metrics as rm

# Custom error objects 
class MetricNameError(Exception):
	def __init__(self,m_name):
		self.m_name = m_name
		self.msg = 'There is no metric '+ self.m_name +' implemented. Choose from ' + str(rm.metric_list) + '.'

class TopKError(Exception):
	def __init__(self, orig_k, pred_k):
		self.orig_k = orig_k
		self.pred_k = pred_k
		self.msg = 'top_k=' + str(self.pred_k) + ' in the prediction and top_k=' + str(self.orig_k) + ' in the original ranking!'

class IntervalCountError(Exception):
	def __init__(self, orig_num, pred_num, pred_name):
		self.orig_num = orig_num
		self.pred_num = pred_num
		self.msg = 'There was ' + str(self.pred_num) + ' interval in the "'+ pred_name +'" prediction but ' + str(self.orig_num) + ' in the original ranking!'


