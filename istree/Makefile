
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
			stree.cpp 
			
TEST_MAIN = main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

TEST_MAIN_OBJECT = $(TEST_MAIN:.cpp=.o)

TARGET = stree


.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE)

all:$(TARGET)

$(TARGET):$(TEST_MAIN_OBJECT) $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)
	
clean:
	rm -rf *.o
	rm -rf $(TARGET)
	