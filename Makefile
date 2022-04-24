FLAGS=-g -Iinclude -Wall
FIND_SRC ?= $(shell if [ `uname` = "Darwin" ] ; then echo find -E src ; else echo find src -regextype posix-extended ; fi)
MAIN_FILE=src/main.cpp
SERVER_FILE=src/server.cpp
CPP_FILES=$(shell find src -name '*.cpp' -and -not \( -wholename '$(MAIN_FILE)' -or -wholename '$(SERVER_FILE)' \) )

test:
	g++ $(FLAGS) $(CPP_FILES) $(MAIN_FILE) -o bin/test.out

server:
	g++ $(FLAGS) $(CPP_FILES) $(SERVER_FILE) -o bin/server.out