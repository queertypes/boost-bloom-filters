CXXFLAGS := -Wall -Wextra -pedantic -std=c++0x -O3 -g
LD_FLAGS := -lpthread -lboost_unit_test_framework
INCLUDE_DIR := ../include
INCLUDES := -I$(INCLUDE_DIR)
EXES := boost_test
all : $(EXES)

%.o : %.cpp $(INCLUDE_DIR)/bloom.hpp
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDES)

boost_test : boost_test.o murmurhash3.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDES) $(LD_FLAGS)

clean:
	rm -f *.o $(EXES)
