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

#define BOOST_TEST_DYN_LINK 1
#define BOOST_TEST_MODULE "Boost Bloom Filter" 1

#include <boost/bloom_filter/dynamic_bloom.hpp>
#include <boost/test/unit_test.hpp>

using boost::bloom_filter::dynamic_bloom_filter;
using boost::bloom_filter::boost_hash;

BOOST_AUTO_TEST_CASE(testUnionFailDifferentSize) {
  dynamic_bloom_filter<size_t> bloom_1(300);
  dynamic_bloom_filter<size_t> bloom_2(299);
  dynamic_bloom_filter<size_t> bloom_union(300);

  bloom_union = bloom_1 | bloom_2;
}

