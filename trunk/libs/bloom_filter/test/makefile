CXXFLAGS := -Wall -Wextra -pedantic -std=c++0x -O3 -g
LD_FLAGS := -lpthread -lboost_unit_test_framework
INCLUDE_DIR := ../../../
INC := ../../../boost/bloom_filter/bloom.hpp
INCLUDES := -I$(INCLUDE_DIR)
EXES := boost_test
all : $(EXES)

%.o : %.cpp $(INC)
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDES)

boost_test : boost_test.o murmurhash3.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDES) $(LD_FLAGS)

clean:
	rm -f *.o $(EXES)
