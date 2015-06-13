msc-thesis
================

Repository for my applied mathematician master thesis

## Introduction

In my [master thesis](http://www.cs.elte.hu/blobs/diplomamunkak/msc_alkmat/2015/beres_ferenc.pdf) I examined how central nodes of a temporal network can be predicted in advance. I used this repository for computing centrality measures on dynamic graphs. The code is based on the [LEMON 1.3.1](http://lemon.cs.elte.hu/trac/lemon) C++ graph library.

In this framework dynamic graphs are partitioned into subgraphs related to time intervals of given length. Then the following centrality measures and graphs statistics can be compared as the graph evolves.

  * Indegree
  * Outdegree
  * Negative beta measure
  * PageRank
  * SALSA

The sources of the centrality features can be found in the [centrality folder](/src/cpp/centrality).

You can clone the repository with the following command
```batch
git clone https://github.com/ferencberes/msc-thesis.git
```
Then change directory into the repository
```batch
cd msc-thesis
```

## Requirements

The project is developed under Unix environment. There are some dependencies that can be installed from a custom script, but the following requirements are needed if you want to use this repository. 

  * g++ 4.8.2 version or higher
  * [SCons](http://www.scons.org/) 2.3.4 or higher

SCons compiles the C++ sources of the project.

## Dependencies

There are several dependencies that are needed for this repository. 

##### Dependencies for the C++ code

  * [LEMON 1.3.1](http://lemon.cs.elte.hu/trac/lemon) (C++ graph library)
  * [CMake](http://www.cmake.org/) 3.2.1 or higher (for compiling Lemon source code)
  * [log4cpp](http://log4cpp.sourceforge.net/) (C++ logging library)

They can be installed from a custom script with the following command.
```batch
./manage/install.sh
```

This script installs all formerly listed dependency into *'src/dep'* folder. 

##### Python dependencies

Python scripts are mostly related to visualization and file formatting tasks. Thus, you can run the C++ code without these Python modules. However, if you want to use custom configuration files for the jobs then the following modules must be installed.

  * [matplotlib](http://matplotlib.org/)
  * numpy
  * json
  * itertools
  * math
  * copy
  * datetime
  * subprocess
  * [jinja2](http://jinja.pocoo.org/docs/dev/)
  * [pytest](http://pytest.org/latest/)

Execution with custom configuration files is explained in the *'Execution using json config files'* section.

## Compile and run tests
After installing C++ dependencies the source code must be compiled and tested. In this project [SCons](http://www.scons.org/) is used for compiling. The repository contains some gtest that check whether the C++ sources are correct. With the execution of the following code you can compile the code and run the tests afterwards.

```batch
./manage/compile_and_test_all.sh
```

The Python scripts can be tested as well. To use this feature you must install [pytest](http://pytest.org/latest/) along with other modules listed in the *'Python dependencies'* section. Then you can run the following command.

```batch
./src/scripts/run_pytest.sh
```

## Execution

Here, I present how you can use the scripts in this repository if you want to run some tests on dynamic graphs. 

##### Input file format

In the examples, we will use the *Enron* graph as input from [Koblenz Network Collection](http://konect.uni-koblenz.de/networks/enron). This is a temporal directed graph related to an email network. You can find the compressed lemon graph format *'enron.lgf.zip'* [here](/resources/example). 

The only compatible file format is *lemon graph format(.lgf)*. It is file format supported by [LEMON](http://lemon.cs.elte.hu/trac/lemon). For each edge the data must contain the source and target vertex, and the Unix epoch timestamps.

For example, here is a portion of the  input:
```
@arcs
labels time
1000 966 0 1004411463
1000 966 1 1004411913
100 100 2 1010781654
100 10756 3 1010687491
...
``` 

##### Convert to lemon graph format

There is a script that you can use to parse *.tsv* or *.csv* files into *.lgf* files. You only need to give the positions for source, target and timestamps in the original file. If you execute this command you will get a hint for parameter usage.

```batch
./src/scripts/parsers/timeline_tsv_to_lgf.py
```

For example, I converted the original Enron *.csv* input format to *.lgf* with a similar command.  
```batch
./src/scripts/parsers/timeline_tsv_to_lgf.py ./enron.csv ./enron.lgf 0 1 3
```

##### Run code from console

After you have compiled the C++ sources successfully you can compute centrality measures on dynamic graphs. You have to decompress *'enron.lgf.zip'* before running any of the following commands.

First execute a command which only loads the graph and extracts some general statistics.
```batch
src/scripts/experiment/runTemporalLemonTest.sh /home/your-user/git/msc-thesis/enron.lgf /home/your-user/git/msc-thesis/enron_test
```

The former command also gives a hint about the full variable setting. 
```
Usage: <input_lgf> <output_folder> <start_time> <delta> <centrality_prev_interval_count> <combine_factor> <graph_stat_prev_interval_count> <enable_multi_edges> <max_num_of_steps> <topK> OPTIONS:(-beta, -degree, -pr, -salsa)
```

Here you have to set several parameters.
   * **input_lgf:** absolute path for input graph in *.lgf* format
   * **output_folder:** absolute path for the output directory
   * **start_time:** the graph analysis will start from this Unix epoch timestamp
   * **delta_time:** the length of the time intervals in seconds 
   * **centrality_prev_interval_count:** compute centrality with the edges of the last given number of intervals 
   * **combine_factor:** set the default -1 value.
   * **graph_stat_prev_interval_count:** set the default -1 value
   * **enable_multi_edges:** enable multiple edges *(true/false)*
   * **max_num_of_steps:** 
   * **topK:** extract *topK* central nodes for each measure
   * **centrality measures:** you can set which measures are computed
       * **degree:** indegree and outdegree
       * **pr:** PageRank (10 iteration, dampening factor is 0.85)
       * **salsa:** SALSA authority and hub scores (10 iteration)
       * **beta:** negative beta measure or "Markovian indegree" (For details see this [article](http://arxiv.org/abs/1308.2140))

For *Enron* graph execute the following command.
```batch
./src/scripts/experiment/runTemporalLemonTest.sh /home/your-user/git/msc-thesis/enron.lgf /home/your-user/git/msc-thesis/enron_test 915148802 7884000 0 -1.0 -1 true 20 10 -degree -pr -salsa -beta
```

After the job ended there is a json file, called *intervals.json*, in the the output folder. It contains all information and results about the given job. The *top_k* central nodes for each centrality measure are included for all intervals as well.

##### Run code with json config files

Documentation for this section is coming soon...

<!--
TODO: introduce json config feature  
some examples in the repo...

## Visualization

##### Changes in the top k central nodes

html table generation

##### Baseline prediction performance

another json example config
-->