msc-thesis
================

Repository for my applied mathematician master thesis

# Introduction

This repository depends on the [LEMON 1.3.1](http://lemon.cs.elte.hu/trac/lemon) graph library.
The sources of the centrality features can be found in the [centrality folder](/src/cpp).

# Requirements

  * g++ (Ubuntu 4.8.2-19ubuntu1) 4.8.2 version or higher
  * cmake version 3.0.2 or higher
  * python simplejson module

# Dependencies
First you must install all dependencies of the project

```batch
./manage/install.sh
```

# Compile and run gtests
After installing dependencies the C++ source code must be compiled and tested.

```batch
./manage/compile_and_test_all.sh
```

