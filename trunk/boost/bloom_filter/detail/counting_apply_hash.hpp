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

#ifndef BOOST_BLOOM_FILTER_COUNTING_APPLY_HASH_HPP
#define BOOST_BLOOM_FILTER_COUNTING_APPLY_HASH_HPP

#include <iostream>

#include <boost/array.hpp>
#include <boost/mpl/at.hpp>

#include <boost/bloom_filter/detail/exceptions.hpp>

namespace boost {
  namespace bloom_filters {
    namespace detail {

      template <typename T, 
		typename Hash, 
		typename Bitset,
		size_t NumBins, size_t BinsPerSlot, 
		size_t BitsPerBin, size_t Mask>
      static size_t 
      get_bits(const T& t, const Bitset slots)
      {
	static Hash hasher;

	const size_t hash_val = hasher(t) % NumBins;
	const size_t pos = hash_val / BinsPerSlot;
	const size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	
	return ((slots[pos] >> offset_bits) & Mask);
      }

      template <size_t N,
	        typename T,
	        size_t NumBins,
		size_t BitsPerBin,
	        class HashFunctions,
		typename Block,
		size_t ArraySize,
		size_t BinsPerSlot>
      struct counting_apply_hash
      {
	static const size_t MASK = 
	  static_cast<Block>(0 - 1) >> (sizeof(Block) * 8 - BitsPerBin);

        static void insert(const T& t, boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, N>::type Hash;
	  static Hash hasher;
	  
	  const size_t hash_val = hasher(t) % NumBins;
	  const size_t pos = hash_val / BinsPerSlot;
	  const size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;
	  ++target_bits;

	  if (target_bits == (1 << BitsPerBin))
	    throw bin_overflow_exception();
	  
	  _slots[pos] |= (target_bits << offset_bits);

	  counting_apply_hash<N-1, T, NumBins, 
			      BitsPerBin, HashFunctions,
			      Block, ArraySize, BinsPerSlot>::insert(t, _slots);
        }

        static void remove(const T& t, boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, N>::type Hash;
	  static Hash hasher;
	  
	  const size_t hash_val = hasher(t) % NumBins;
	  const size_t pos = hash_val / BinsPerSlot;
	  const size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;

	  if (target_bits == 0)
	    throw bin_underflow_exception();
	  
	  --target_bits;

	  _slots[pos] |= (target_bits << offset_bits);

	  counting_apply_hash<N-1, T, NumBins, 
			      BitsPerBin, HashFunctions,
			      Block, ArraySize, BinsPerSlot>::remove(t, _slots);
	}

        static bool contains(const T& t, const boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, N>::type Hash;
	  static Hash hasher;
	  
	  const size_t hash_val = hasher(t) % NumBins;
	  const size_t pos = hash_val / BinsPerSlot;
	  const size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  const size_t target_bits = (_slots[pos] >> offset_bits) & MASK;

	  return ((target_bits != 0) && 
		  counting_apply_hash<N-1, T, NumBins, 
				      BitsPerBin, HashFunctions,
				      Block, ArraySize, BinsPerSlot>::contains(t, _slots));
        }
      };

      template <typename T,
	        size_t NumBins,
		size_t BitsPerBin,
	        class HashFunctions,
		typename Block,
		size_t ArraySize,
		size_t BinsPerSlot>
      struct counting_apply_hash<0, T, NumBins, BitsPerBin, HashFunctions,
				 Block, ArraySize, BinsPerSlot>
      {
	static const size_t MASK = 
	  static_cast<Block>(0 - 1) >> (sizeof(Block) * 8 - BitsPerBin);

        static void insert(const T& t, 
			   boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	  static Hash hasher;

	  const size_t hash_val = hasher(t) % NumBins;
	  const size_t pos = hash_val / BinsPerSlot;
	  const size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;

	  ++target_bits;

	  if (target_bits == (1 << BitsPerBin)) 
	    throw bin_overflow_exception();
	  
	  _slots[pos] &= ~(MASK << offset_bits);
	  _slots[pos] |= (target_bits << offset_bits);
        }

        static void remove(const T& t, 
			   boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	  static Hash hasher;

	  const size_t hash_val = hasher(t) % NumBins;
	  const size_t pos = hash_val / BinsPerSlot;
	  const size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;

	  if (target_bits == 0)
	    throw bin_underflow_exception();

	  --target_bits;

	  _slots[pos] &= ~(MASK << offset_bits);
	  _slots[pos] |= (target_bits << offset_bits);
	}

        static bool contains(const T& t, 
			     const boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;

	  size_t target_bits = 
	    get_bits<T, Hash, boost::array<Block, ArraySize>,
		     NumBins, BinsPerSlot, BitsPerBin, MASK>(t, _slots);

	  return (target_bits != 0);
        }
      };
    } // namespace detail
  } // namespace bloom_filter
} // namespace boost
#endif
