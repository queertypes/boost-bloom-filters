#include <iostream>
#include "../lib/murmurhash3/MurmurHash3.h"
#include "../include/bloom.hpp"

typedef int BloomType;

template <typename T, size_t Seed>
struct OHash {
  static size_t hash(const T& t) {
    size_t out = 0;
    MurmurHash3_x86_32(static_cast<const void *const>(&t), 
		       sizeof(t),
		       Seed,
		       &out);
    return out;
  }
};

typedef std::tuple<OHash<BloomType, 2>,
		   OHash<BloomType, 3>,
		   OHash<BloomType, 5>,
		   OHash<BloomType, 7>,
		   OHash<BloomType, 11>,
		   OHash<BloomType, 13>,
		   OHash<BloomType, 17>,
		   OHash<BloomType, 19>> EightHashFunctions_O;

//typedef std::tuple<MurmurHash3<BloomType, 19>> SingleHashFunction;

int main()
{
  static const BloomType INSERT_VAL = 100;
  static const size_t SEARCH_SPACE = 10000000;
  static const size_t FILTER_SIZE = 64; 
  size_t collisions = 0;
  bloom_filter<BloomType, FILTER_SIZE, EightHashFunctions_O> bloom;

  std::cout << "bloom size " << sizeof(bloom) << std::endl;
  bloom.insert(INSERT_VAL);
  for (size_t i = 0; i < SEARCH_SPACE; ++i) {
    if (bloom[i] && i != INSERT_VAL) ++collisions;
  }

  std::cout << collisions << " collisions" << std::endl;
  bloom.clear();
  
  return 0;
}
