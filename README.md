msc-thesis
================

Repository for my applied mathematician master thesis

## Introduction

In my [master thesis](http://www.cs.elte.hu/blobs/diplomamunkak/msc_alkmat/2015/beres_ferenc.pdf) I examined how central nodes of a temporal network can be predicted in advance. I used this repository for computing centrality measures on dynamic graphs. The code is based on the [LEMON 1.3.1](http://lemon.cs.elte.hu/trac/lemon) C++ graph library.

In this framework dynamic graphs are partitioned into subgrpahs related to time intervals of given length. Then the following centrality measures and graphs statistics can be compared as the graph evolves.

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

The project is developed under unix environment. There are some dependencies that can be installed from a custom script, but the following requirements are needed if you want to use this repository. 

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

Python scripts are mostly related to visualization and file formatting tasks. Thus, you can run the C++ code without these Python modules. However, if you want to use custom config files for the jobs then the following modules must be installed. 

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

Execution with custom config files is explained in the *'Execution using json config files'* section.

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
##### Input example

from koblenz sample input

##### Convert to lemon graph format

or

```batch
wget cs.elte home-ból...
```

##### Run code from console

##### Run code with json config files

some examples in the repo...

## Visualization

##### Changes in the top k central nodes

html table generation

##### Baseline prediction performance

another json example config