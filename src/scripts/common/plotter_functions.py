from matplotlib import pyplot as plt
from matplotlib import dates
import itertools as it
import numpy as np
	
def init_settings():
	setting_list = []
	setting_list.append(it.cycle(('^', 'o', 'v', 's', 'p', 'x')))
	setting_list.append(it.cycle(('1.0', '1.0', '2', '2')))
        setting_list.append(it.cycle(('-', '--', '-.', ':')))
        #colors: blue, green, red, black, orange, cyan, purple
	setting_list.append(it.cycle(('#0000cc', '#339933', '#ff0000', '#000000', '#ff9900', '#0099ff', '#9900ff')))
	return setting_list

def set_x_axis_config(axis_obj, x_args, x_axis_label, x_sticks_rate):
	axis_obj.set_xlabel(x_axis_label)
	epsilon = 0.01
	x_lower = x_args[0]
	x_upper = x_args[len(x_args) - 1]
        plt.xlim(x_lower - epsilon, x_upper + epsilon)
	x_delta =  float(x_upper - x_lower) / (len(x_args) - 1)
	axis_obj.xaxis.set_ticks(np.arange(x_lower, x_upper + x_delta, x_delta * x_sticks_rate))

def set_y_axis_config(axis_obj, y_axis_label, y_limit, y_sticks_rate, y_grid_enabled):
 	axis_obj.set_ylabel(y_axis_label)
	if y_limit != 0:
		epsilon = 0.01
		y_lower = 0.0
		y_upper = y_limit
        	plt.ylim(y_lower, y_upper + epsilon)
		y_delta =  float(y_upper - y_lower) / y_sticks_rate
		axis_obj.yaxis.set_ticks(np.arange(y_lower, y_upper + y_delta, y_delta))
	if y_grid_enabled:
     		axis_obj.yaxis.grid()

def draw_line_plots(x_args, plot_data, title_text, x_axis_label, y_axis_label, x_sticks_rate, y_sticks_rate, y_limit, plot_dir, filename):
        [markers,line_widths,line_styles,colors] = init_settings()
	fig_ndcg = plt.figure()
        plt.title(title_text)
        ax = fig_ndcg.add_subplot(111)
	set_x_axis_config(ax, x_args, x_axis_label, x_sticks_rate)
	set_y_axis_config(ax, y_axis_label, y_limit, y_sticks_rate, True)
	for item in plot_data:
                plt.plot(x_args, item[1], color=colors.next(), marker=markers.next(), linestyle=line_styles.next(), linewidth=line_widths.next(), label=item[0])
        handles, labels = ax.get_legend_handles_labels()
        lgd = ax.legend(handles, labels, loc='center left', bbox_to_anchor=(1, 0.5))
        fig_ndcg.savefig(plot_dir + '/' + filename, bbox_extra_artists=(lgd,), bbox_inches='tight')
	plt.clf()

def draw_error_plots(x_sticks_vector, plot_data, title_text, x_axis_label, y_axis_label, y_sticks_rate, y_limit, plot_dir, filename):
        [markers,line_widths,line_styles,colors] = init_settings()
	perturbance = 0.1
	fig_ndcg = plt.figure()
        plt.title(title_text)
        ax = fig_ndcg.add_subplot(111)
	set_y_axis_config(ax, y_axis_label, y_limit, y_sticks_rate, True)
	ax.set_xlabel(x_axis_label)
	counter = 0
	for item in plot_data:
		stats=item[1]
		x=[]
		lower_diff=[]
		upper_diff=[]
		pos=0
		marker = markers.next()
		line_s = line_styles.next()
		color_l = colors.next()
		for i in range(0,len(x_sticks_vector)):
			pos += 1
			x.append(pos + counter * perturbance)
			lower_diff.append(stats[0][i]-stats[1][i])
			upper_diff.append(stats[2][i]-stats[0][i])
		plt.errorbar(x,stats[0], yerr=[lower_diff,upper_diff], c=color_l, fmt=marker, label=item[0])
		counter += 1	

        handles, labels = ax.get_legend_handles_labels()
       	x_sticks_vector.insert(0,'') 
	ax.set_xticklabels(x_sticks_vector)
	lgd = ax.legend(handles, labels, loc='center left', bbox_to_anchor=(1, 0.5))
        fig_ndcg.savefig(plot_dir + '/' + filename, bbox_extra_artists=(lgd,), bbox_inches='tight')
	plt.clf()

def draw_plots_with_time(x_args, plot_data, title_text, x_axis_label, y_axis_label, x_sticks_rate, plot_dir, filename):
	[markers,line_widths,line_styles,colors] = init_settings()
	fig = plt.figure()
	plt.title(title_text)
	ax = fig.add_subplot(111)
	set_x_axis_config(ax, x_args, x_axis_label, x_sticks_rate)
	set_y_axis_config(ax, y_axis_label, 0, -1, True)
	for plot_item in plot_data:
		ax.plot(x_args, plot_item[1], color=colors.next(), marker=markers.next(), linestyle=line_styles.next(), linewidth=line_widths.next(), label=plot_item[0])
	handles, labels = ax.get_legend_handles_labels()
	hfmt = dates.DateFormatter('%Y-%m-%d %H:%M:%S')
	ax.xaxis.set_major_formatter(hfmt)
	lgd = ax.legend(handles, labels, loc='center left', bbox_to_anchor=(1, 0.5))	
	plt.gcf().autofmt_xdate()	
	fig.savefig(plot_dir + '/' + filename, bbox_extra_artists=(lgd,), bbox_inches='tight')
	plt.clf()

def draw_plots_from_pairs(plot_data, title_text, x_axis_label, y_axis_label, plot_dir, filename):
	fig = plt.figure()
	plt.title(title_text)
	ax = fig.add_subplot(111)
	ax.set_xlabel(x_axis_label)
	ax.set_ylabel(y_axis_label)
	for plot_item in plot_data:
		ax.plot(*zip(*plot_item[1]), color=colors.next(), marker=markers.next(), linestyle=line_styles.next(), linewidth=line_widths.next(), ls='', mew=2, label=plot_item[0])
	#ax.plot(*zip(*data_1), marker='x', color='r', ls='', mew=2, label=data_1_label)
	handles, labels = ax.get_legend_handles_labels()
	lgd = ax.legend(handles, labels, loc='center left', bbox_to_anchor=(1, 0.5))	
	fig.savefig(plot_dir + '/' + filename, bbox_extra_artists=(lgd,), bbox_inches='tight')
	plt.clf()

