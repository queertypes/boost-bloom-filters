CXXFLAGS := -Wall -Wextra -pedantic -std=c++0x -O3 -g
ST_LIBS := ../lib/murmurhash3/MurmurHash3.o
INCLUDE_DIR := ../include
INCLUDES := $(INCLUDE_DIR)/bloom.hpp $(INCLUDE_DIR)/hash.hpp
all : test_bloom

%.o : %.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $<

test_bloom : test_bloom.o $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(ST_LIBS)

clean:
	rm -f *.o test_bloom
