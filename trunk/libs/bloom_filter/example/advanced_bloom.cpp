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

// advanced Boost.BloomFilter program
#include <boost/bloom_filter/bloom.hpp>
#include <iostream>
using namespace boost::bloom_filter;
using namespace std;

int main () {
  typedef boost::mpl::vector<boost_hash<int, 1>,
                             boost_hash<int, 2>,
			     boost_hash<int, 3> > HashFns;

  static const size_t INSERT_MAX = 5000;
  static const size_t CONTAINS_MAX = 10000;
  static const size_t NUM_BITS = 1024;

  bloom_filter<int, NUM_BITS, HashFns> bloom;
  size_t collisions = 0;

  cout << "false positive rate: "
       << bloom.false_positive_rate()
       << endl;

  for (int i = 0; i < INSERT_MAX; ++i) {
    bloom.insert(i);
  }

  for (int i = INSERT_MAX; i < CONTAINS_MAX_MAX; ++i) {
    if (bloom.contains(i)) ++collisions;
  }

  cout << "false positive rate: "
       << bloom.false_positive_rate()
       << endl;

  cout << "collisions: " << collisions << endl;

  return 0;
}
