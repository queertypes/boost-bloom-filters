CXXFLAGS := -Wall -Wextra -pedantic -std=c++0x -O3 -g
ST_LIBS := lib/murmurhash3/MurmurHash3.o
all : test_bloom

%.o : %.cpp bloom.hpp hash.hpp
	$(CXX) $(CXXFLAGS) -c $<

test_bloom : test_bloom.o bloom.hpp hash.hpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(ST_LIBS)

clean:
	rm -f *.o test_bloom
