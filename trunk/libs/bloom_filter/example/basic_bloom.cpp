//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Alejandro Cabrera 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bloom_filter for documentation.
//
//////////////////////////////////////////////////////////////////////////////

// introductory Boost.BloomFilter program
#include <boost/bloom_filter/bloom.hpp>
#include <iostream>
using namespace boost::bloom_filter;
using namespace std;

int main () {
  static const size_t INSERT_MAX = 5000;
  static const size_t CONTAINS_MAX = 10000;
  static const size_t NUM_BITS = 512;

  bloom_filter<int, NUM_BITS> bloom;
  size_t collisions = 0;

  for (int i = 0; i < INSERT_MAX; ++i) {
    bloom.insert(i);
  }

  for (int i = INSERT_MAX; i < CONTAINS_MAX_MAX; ++i) {
    if (bloom.contains(i)) ++collisions;
  }

  cout << "collisions: " << collisions << endl;

  return 0;
}
