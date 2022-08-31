NAME  			:= simplemath
CXX				:= g++
CXXFLAGS		:= -Wall -Werror -Wextra -std=c++20
INCLUDE_DIR		:= -Iinclude


default: purge setup shared static
setup:
	@mkdir -p obj lib bin


# make shared library
shared: CXXFLAGS += -fPIC -shared
shared:
	@mkdir -p lib
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -o lib/lib$(NAME).so src/*.cpp


# make static library
sources := $(wildcard src/*.cpp)
srcobjs := ${sources:.cpp=.o}
objects := $(patsubst src/%, obj/%, $(srcobjs))
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@
static: setup $(objects)
	ar rcvs lib/lib$(NAME).a obj/*.o


# test library files
test: setup
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) tests/add.cpp src/add.cpp -o bin/test_add.out
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) tests/subtract.cpp src/subtract.cpp -o bin/test_subtract.out
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) tests/divide.cpp src/divide.cpp -o bin/test_divide.out
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) tests/multiply.cpp src/multiply.cpp -o bin/test_multiply.out



# clean build files
purge: clean
	rm -rvf lib
clean:
	rm -rvf obj bin


# ____ used when using the library static/shared/direct with an executable (generally for testing) ____
LIBRARY_SRC := src/*.cpp
LIBRARY_DIR	:= -Llib
TARGET 		:= bin/main

use_direct:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) main.cpp $(LIBRARY_SRC) -o $(TARGET)

use_static: LDFLAGS	:= -lsimplemath
use_static:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $(LIBRARY_DIR) -static main.cpp -o $(TARGET) $(LDFLAGS)

use_shared: LDFLAGS	:= lib/libsimplemath.so
use_shared:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) $(LIBRARY_DIR) main.cpp -o $(TARGET) $(LDFLAGS)