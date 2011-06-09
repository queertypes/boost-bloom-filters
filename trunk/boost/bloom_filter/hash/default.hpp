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

#ifndef BOOST_BLOOM_FILTER_HASH_HPP_
#define BOOST_BLOOM_FILTER_HASH_HPP_ 1
/**
 * \author Alejandro Cabrera
 * \brief A default Hasher to be used with the Bloom filter.
 */
#include <boost/functional/hash.hpp>

namespace boost {
  namespace bloom_filter {
  template <typename T, size_t Seed>
    struct boost_hash {
      size_t operator()(const T& t) {
        size_t seed = Seed;
        boost::hash_combine(seed, t);
        return seed;
      }
    };
  }
}
#endif
