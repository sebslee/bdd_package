# README #

This README contains basic information on the BDD Package implemented
by Olga Wagner and Sebastian Lee

*********************************************************************
*SUMMARY

This repository contains a minimal BDD package implemented in C++. The package
implements the fundamental manipulation methods for ROBDDs.
The package also includes a basic test suite.
NOTE: This package version is not speed-enhanced yet. Performance improvements
will be deployed in later versions.

*VERSION
Current version 0.01

*CONTENT

1.-Cloning the Repository
2.-Compiling
3.-Running the basic test suite
4.-Generating documentation
5.-Contact

**********************************************************************

1.- Cloning the repo:

To clone the current master branch:

git clone https://sleebarr@bitbucket.org/sleebarr/vdscp_01.git

2.- Compiling

The current implementation requires Cmake with at least version 2.8 installed.
You can generate the target files by:

cmake $REPO_ROOT/CMakeLists.txt && make

3.- Runnning the basic test suite

Current implementation generates two executable files.

VDSProject - main obj file for main.cpp. No Gtest required.
VDSProject_test - obj file for $REPO_ROOT/src/test/main_test.cpp requiring Gtest framework. Run this executable for testing.


4.- Generating documentation

To create documentation of source code, first step is generating project-specyfiic doxygen configuration file ($ doxygen -g my_proj.conf).

In new generated configuration file we can edit some options:

	# document all entities in the project.
	EXTRACT_ALL            = YES

	# document all static members of a file.
	EXTRACT_STATIC         = YES

	# specify the root directory that contains the project's source files.
	INPUT                  = /home/xmodulo/source

	# search sub-directories for all source files.
	RECURSIVE              = YES

	# include the body of functions and classes in the documentation.
	INLINE_SOURCES         = YES

	# generate visualization graph by using dot program (part of graphviz 	package).
	HAVE_DOT               = YES

Next step is to run doxygen with configuration file ($ doxygen my_proj.conf)

5.- Contact

Repo owners/admin

Olga Wagner   : olgawglzy@gmail.com
Sebastian Lee : sbslee@gmail.com
