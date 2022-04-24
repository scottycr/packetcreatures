FLAGS=-g -Iinclude -Wall
FIND_SRC ?= $(shell if [ `uname` = "Darwin" ] ; then echo find -E src ; else echo find src -regextype posix-extended ; fi)

TEST_FILE=src/main.cpp
SERVER_FILE=src/server.cpp
CLIENT1_FILE=src/client1.cpp
CLIENT2_FILE=src/client2.cpp

CPP_FILES=$(shell find src -name '*.cpp' -and -not \( -wholename '$(TEST_FILE)' -or -wholename '$(SERVER_FILE)' \
			-or -wholename '$(CLIENT1_FILE)' -or -wholename '$(CLIENT2_FILE)' \) )

all: server client1 client2 

client: client1 client2

test:
	g++ $(FLAGS) $(CPP_FILES) $(MAIN_FILE) -o bin/test.out

server:
	g++ $(FLAGS) $(CPP_FILES) $(SERVER_FILE) -o bin/server.out

client1:
	g++ $(FLAGS) $(CPP_FILES) $(CLIENT1_FILE) -o bin/client1.out

client2:
	g++ $(FLAGS) $(CPP_FILES) $(CLIENT2_FILE) -o bin/client2.out