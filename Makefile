FLAGS=-g -Iinclude -Wall
FIND_SRC ?= $(shell if [ `uname` = "Darwin" ] ; then echo find -E src ; else echo find src -regextype posix-extended ; fi)

TEST_FILE=main.cpp
SERVER_FILE=server.cpp
CLIENT_FILE=client.cpp
CLIENT1_FILE=client1.cpp
CLIENT2_FILE=client2.cpp

CPP_FILES=$(shell find src -name '*.cpp' -and -not \( -name '$(TEST_FILE)' -or -name '$(SERVER_FILE)' \
			-or -name '$(CLIENT1_FILE)' -or -name '$(CLIENT2_FILE)' -or -name '$(CLIENT_FILE)' \) )

all: server client 

clients: client1 client2

test:
	g++ $(FLAGS) $(CPP_FILES) src/$(MAIN_FILE) -o bin/test.out

server:
	g++ $(FLAGS) $(CPP_FILES) src/$(SERVER_FILE) -o bin/server.out

client: 
	g++ $(FLAGS) $(CPP_FILES) src/$(CLIENT_FILE) -o bin/client.out
	
client1:
	g++ $(FLAGS) $(CPP_FILES) src/$(CLIENT1_FILE) -o bin/client1.out

client2:
	g++ $(FLAGS) $(CPP_FILES) src/$(CLIENT2_FILE) -o bin/client2.out