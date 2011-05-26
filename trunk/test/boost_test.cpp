#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Boost Bloom Filter" 1
#include <iostream>

#include <bloom.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(defaultConstructor) {
  typedef std::tuple<
    OHash <int, 2>,
    OHash<int, 3>,
    OHash<int, 5>,
    OHash<int, 7>,
    OHash<int, 11>,
    OHash<int, 13>,
    OHash<int, 17>,
    OHash<int, 19>> EightHashFunctions_O;
  
  bloom_filter<int, 8> bloom1;
  bloom_filter<int, 8, EightHashFunctions_O> bloom2;
}

BOOST_AUTO_TEST_CASE(assignment)
{
  bloom_filter<int, 8> bloom1;
  bloom_filter<int, 8> bloom2;

  for (size_t i = 0; i < 200; ++i) {
    bloom1.insert(i);
    BOOST_CHECK_EQUAL(bloom1.contains(i), true);
  }

  bloom2 = bloom1;

  for (size_t i = 0; i < 200; ++i) {
    BOOST_CHECK_EQUAL(bloom2.contains(i), true);
  }
}

BOOST_AUTO_TEST_CASE(size) {
  bloom_filter<size_t, 8> bloom_8;
  bloom_filter<size_t, 256> bloom_256;
  bloom_filter<size_t, 2048> bloom_2048;
  
  BOOST_CHECK_EQUAL(bloom_8.size(), 8ul);
  BOOST_CHECK_EQUAL(bloom_256.size(), 256ul);
  BOOST_CHECK_EQUAL(bloom_2048.size(), 2048ul);
}

BOOST_AUTO_TEST_CASE(contains) {
  bloom_filter<size_t, 8> bloom;  

  bloom.insert(1);
  BOOST_CHECK_EQUAL(bloom.contains(1), true);
}

BOOST_AUTO_TEST_CASE(containsOperator) {
  bloom_filter<size_t, 8> bloom;  

  bloom.insert(1);
  BOOST_CHECK_EQUAL(bloom.contains(1), true);
}

BOOST_AUTO_TEST_CASE(doesNotContain) {
  bloom_filter<size_t, 8> bloom;  

  BOOST_CHECK_EQUAL(bloom.contains(1), false);
}

BOOST_AUTO_TEST_CASE(insertNoFalseNegatives) {
  bloom_filter<size_t, 2048> bloom;

  for (size_t i = 0; i < 100; ++i) {
    bloom.insert(i);
    BOOST_CHECK_EQUAL(bloom.contains(i), true);
  }
}

BOOST_AUTO_TEST_CASE(clear) {
  bloom_filter<size_t, 8> bloom;  

  for (size_t i = 0; i < 1000; ++i)
    bloom.insert(i);

  bloom.clear();
  BOOST_CHECK_EQUAL(bloom.contains(1), false);
}

BOOST_AUTO_TEST_CASE(testUnion) {
  bloom_filter<size_t, 32> bloom_1;
  bloom_filter<size_t, 32> bloom_2;
  bloom_filter<size_t, 32> bloom_union;

  for (size_t i = 0; i < 100; ++i) 
    bloom_1.insert(i);
  
  for (size_t i = 100; i < 200; ++i) 
    bloom_2.insert(i);

  bloom_union = bloom_1 | bloom_2;

  for (size_t i = 0; i < 200; ++i)
    BOOST_CHECK_EQUAL(bloom_union.contains(i), true);
}

BOOST_AUTO_TEST_CASE(testUnionAssign) {
  bloom_filter<size_t, 32> bloom_1;
  bloom_filter<size_t, 32> bloom_2;

  for (size_t i = 0; i < 100; ++i) 
    bloom_1.insert(i);
  
  bloom_2 |= bloom_1;

  for (size_t i = 0; i < 100; ++i)
    BOOST_CHECK_EQUAL(bloom_2.contains(i), true);
}

BOOST_AUTO_TEST_CASE(testIntersect) {
  bloom_filter<size_t, 8> bloom_1;
  bloom_filter<size_t, 8> bloom_2;
  bloom_filter<size_t, 8> bloom_intersect;

  for (size_t i = 0; i < 100; ++i) 
    bloom_1.insert(i);
  
  for (size_t i = 100; i < 200; ++i) 
    bloom_2.insert(i);

  bloom_intersect = bloom_1 & bloom_2;

  for (size_t i = 0; i < 200; ++i)
    BOOST_CHECK_EQUAL(bloom_intersect.contains(i), false);
}

BOOST_AUTO_TEST_CASE(testIntersectAssign) {
  bloom_filter<size_t, 8> bloom_1;
  bloom_filter<size_t, 8> bloom_2;

  for (size_t i = 0; i < 100; ++i) 
    bloom_1.insert(i);
  
  bloom_2 &= bloom_1;

  for (size_t i = 0; i < 100; ++i)
    BOOST_CHECK_EQUAL(bloom_2.contains(i), false);
}

BOOST_AUTO_TEST_CASE(collisionBenchmark) {
  typedef std::tuple<
    OHash <size_t, 2>,
    OHash<size_t, 3>,
    OHash<size_t, 5>,
    OHash<size_t, 7>,
    OHash<size_t, 11>,
    OHash<size_t, 13>,
    OHash<size_t, 17>,
    OHash<size_t, 19>> EightHashFunctions_O;

  static const size_t INSERT_VAL = 100;
  static const size_t SEARCH_SPACE = 10000000;
  static const size_t FILTER_SIZE = 64; 
  size_t collisions = 0;
  bloom_filter<size_t, FILTER_SIZE, EightHashFunctions_O> bloom;

  std::cout << "bloom size " << bloom.size() << std::endl;
  bloom.insert(INSERT_VAL);
  for (size_t i = 0; i < SEARCH_SPACE; ++i) {
    if (bloom[i] && i != INSERT_VAL) ++collisions;
  }

  std::cout << collisions << " collisions" << std::endl;
  bloom.clear();
}
