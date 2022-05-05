FLAGS=-g -Iinclude -Wall
FIND_SRC ?= $(shell if [ `uname` = "Darwin" ] ; then echo find -E src ; else echo find src -regextype posix-extended ; fi)

SERVER_FILE=server.cpp
CLIENT_FILE=client.cpp

CPP_FILES=$(shell find src -name '*.cpp' -and -not \( -name '$(TEST_FILE)' -or -name '$(SERVER_FILE)' \
			-or -name '$(CLIENT1_FILE)' -or -name '$(CLIENT2_FILE)' -or -name '$(CLIENT_FILE)' \) )

all: server client 

server:
	g++ $(FLAGS) $(CPP_FILES) src/$(SERVER_FILE) -o bin/server.out

client: 
	g++ $(FLAGS) $(CPP_FILES) src/$(CLIENT_FILE) -o bin/client.out
