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
#ifndef BOOST_BLOOM_FILTER_APPLY_HASH_HPP
#define BOOST_BLOOM_FILTER_APPLY_HASH_HPP

#include <boost/mpl/at.hpp>

namespace boost {
  namespace detail {
    template <size_t N,
	      typename T,
	      size_t Size,
	      class HashFunctions>
    struct apply_hash
    {
      static void insert(const T& t, std::bitset<Size>& _bits) {
	typedef typename boost::mpl::at_c<HashFunctions, N>::type Hash;
	static Hash hasher;
	_bits[hasher(t) % Size] = true;
	apply_hash<N-1, T, Size, HashFunctions>::insert(t, _bits);
      }
    
      static bool contains(const T& t, const std::bitset<Size>& _bits) {
	typedef typename boost::mpl::at_c<HashFunctions, N>::type Hash;
	static Hash hasher;
	return (_bits[hasher(t) % Size] && 
		apply_hash<N-1, T, Size, HashFunctions>::contains(t, _bits));
      }
    };

    template <typename T,
	      size_t Size,
	      class HashFunctions>
    struct apply_hash<0, T, Size, HashFunctions>
    {    
      static void insert(const T& t, std::bitset<Size>& _bits) {
	typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	static Hash hasher;
	_bits[hasher(t) % Size] = true;
      }
    
      static bool contains(const T& t, const std::bitset<Size>& _bits) {
	typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	static Hash hasher;
	return (_bits[hasher(t) % Size]);
      }
    };
  }
}
#endif
