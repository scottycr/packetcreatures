FLAGS=-g -Iinclude
FIND_SRC ?= $(shell if [ `uname` = "Darwin" ] ; then echo find -E src ; else echo find src -regextype posix-extended ; fi)
CPP_FILES=$(shell find src -name '*.cpp')

test:
	g++ $(FLAGS) $(CPP_FILES) -o bin/test.out