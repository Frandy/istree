
CC = g++-4.7

GCCDIR = /usr/local/gcc47/gcc-4.7
CPP_INCLUDE_PATH = $(GCCDIR)/include
CPP_LIBRARY_PATH = $(GCCDIR)/lib

OPT = -O3

#PROFILE = -pg 

INCLUDE = -I . -I ./unitest

CFLAGS = $(OPT) $(PROFILE) -Wall -std=c++0x -I $(CPP_INCLUDE_PATH)
LFLAGS = $(OPT) $(PROFILE) -std=c++0x -L $(CPP_LIBRARY_PATH) 

		  
SOURCES = 	graph.cpp \
			edge.cpp \
			symbol.cpp \
			symbFromFile.cpp \
			stnode.cpp \
			stree.cpp \
			orderGraph.cpp \
			testOrderAndNormal.cpp
			
TEST_MAIN = main.cpp

TEST_ORDER = testOrder.cpp

TEST_NORMAL = testNormal.cpp

TEST_PERMUTATION = test_permutation.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

TEST_MAIN_OBJECT = $(TEST_MAIN:.cpp=.o)

TEST_ORDER_OBJECT = $(TEST_ORDER:.cpp=.o)

TEST_NORMAL_OBJECT = $(TEST_NORMAL:.cpp=.o)

TEST_PERMUTATION_OBJECT = $(TEST_PERMUTATION:.cpp=.o)

TARGET = stree

TARGET_ORDER = test_order

TARGET_NORMAL = test_normal

TARGET_PERMUTATION = test_permutation

.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE)

all:$(TARGET) $(TARGET_ORDER) $(TARGET_NORMAL) $(TARGET_PERMUTATION)

$(TARGET):$(TEST_MAIN_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)
	
$(TARGET_NORMAL):$(TEST_NORMAL_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)
	
$(TARGET_ORDER):$(TEST_ORDER_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)		
	
$(TARGET_PERMUTATION):$(TEST_PERMUTATION_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)			
	
clean:
	rm -rf *.o
	rm -rf $(TARGET)
	