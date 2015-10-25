
## written by Jun Zeng, 10/20/2015 ##

BIN_DIR=bin
LIB_DIR=lib
INC_DIR=include
TEST_DIR=src/test
SRC_DIR=src/src

BIN_SRC:=$(wildcard $(SRC_DIR)/*.cpp)
TEST_SRC:=$(wildcard $(TEST_DIR)/*.cpp)
LIB_SRC:=$(wildcard $(LIB_DIR)/*.cpp)
INC_SRC:=$(wildcard $(INC_DIR)/*.h)

BINS:=$(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%, $(BIN_SRC))
TESTS:=$(patsubst $(TEST_DIR)/%.cpp, test/%, $(TEST_SRC))

BIN_OBJ:=$(patsubst $(SRC_DIR)/%.cpp, obj/src/%.o, $(BIN_SRC))
TEST_OBJ:=$(patsubst $(TEST_DIR)/%.cpp, obj/test/%.o, $(TEST_SRC))
LIB_OBJ:=$(patsubst $(LIB_DIR)/%.cpp, obj/lib/%.o, $(LIB_SRC)) $(wildcard *.a)

CC=g++
CFLAGS= -lpthread -O2 -g -Wall -std=c++11 -I$(INC_DIR)

.PHONY: all test  

## the source code part ##

all: $(BINS) 

$(BINS): $(BIN_OBJ) $(LIB_OBJ) 
	@mkdir -p $(dir $@)
	$(CC) -o $@ $< $(LIB_OBJ) $(CFLAGS)
	@./$@
	@echo "[INFO]: \".mdtmApp.xml\" file is produced successfully, and placed in root directory"

obj/src/%.o: src/src/%.cpp $(wildcard include/*.h)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

## the function test part ##

test: $(TESTS) 

$(TESTS): $(TEST_OBJ) $(LIB_OBJ) 
	@mkdir -p test
	$(CC) -o $@ $(addprefix obj/test/, $(notdir $@).o) $(LIB_OBJ) $(CFLAGS) 

obj/test/%.o: src/test/%.cpp $(wildcard include/*.h)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

obj/lib/%.o: $(LIB_DIR)/%.cpp $(wildcard include/*.h)   
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<


.PHONY: clean

clean:
	rm -rf obj test bin



