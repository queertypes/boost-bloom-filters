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

namespace boost {
  namespace bloom_filters {
    namespace detail {

      /*************************************************************************
       *                          static bloom filter
       ************************************************************************/
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
	  
	  size_t hash_val = Hash(t) % NumBins;
	  size_t pos = hash_val / BinsPerSlot;
	  size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;
	  ++target_bits;	  
	  _slots[pos] |= (target_bits << offset_bits);

	  counting_apply_hash<N-1, T, NumBins, 
			      BitsPerBin, HashFunctions,
			      Block, ArraySize, BinsPerSlot>::insert(t, _slots);
        }

        static void remove(const T& t, boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, N>::type Hash;
	  static Hash hasher;
	  
	  size_t hash_val = hasher(t) % NumBins;
	  size_t pos = hash_val / BinsPerSlot;
	  size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;
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
	  
	  size_t hash_val = hasher(t) % NumBins;
	  size_t pos = hash_val / BinsPerSlot;
	  size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;

	  return ((target_bits > 0) && 
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

        static void insert(const T& t, boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	  static Hash hasher;

	  /*
	  std::cout << "(meta) NumBins: " << NumBins
		    << " BitsPerBin: " << BitsPerBin 
		    << " ArraySize: " << ArraySize 
		    << " BinsPerSlot: " << BinsPerSlot
		    << " incoming value: " << t << "\n";
	  */

	  size_t hash_val = hasher(t) % NumBins;
	  size_t pos = hash_val / BinsPerSlot;
	  size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;
	  ++target_bits;	  
	  _slots[pos] &= ~(MASK << offset_bits);
	  _slots[pos] |= (target_bits << offset_bits);

	  /*
	  std::cout << "(insert) updated bits at pos " << pos 
		    << " at bit offset " << offset_bits
		    << " using mask " << MASK
		    << " hashed as " << hash_val
		    << " from " << target_bits - 1 
		    << " to " << target_bits << "\n";
	  */
        }

        static void remove(const T& t, boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	  static Hash hasher;

	  /*
	  std::cout << "(meta) NumBins: " << NumBins
		    << " BitsPerBin: " << BitsPerBin 
		    << " ArraySize: " << ArraySize 
		    << " BinsPerSlot: " << BinsPerSlot
		    << " incoming value: " << t << "\n";
	  */
	  
	  size_t hash_val = hasher(t) % NumBins;
	  size_t pos = hash_val / BinsPerSlot;
	  size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;
	  --target_bits;
	  _slots[pos] &= ~(MASK << offset_bits);
	  _slots[pos] |= (target_bits << offset_bits);

	  /*
	  std::cout << "(remove) updated bits at pos " << pos 
		    << " at bit offset " << offset_bits
		    << " using mask " << MASK
		    << " hashed as " << hash_val
		    << " from " << target_bits + 1 
		    << " to " << target_bits << "\n";
	  */
	}

        static bool contains(const T& t, const boost::array<Block, ArraySize>& _slots) 
	{
	  typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	  static Hash hasher;
	  
	  /*
	  std::cout << "(meta) NumBins: " << NumBins
		    << " BitsPerBin: " << BitsPerBin 
		    << " ArraySize: " << ArraySize 
		    << " BinsPerSlot: " << BinsPerSlot 
		    << " incoming value: " << t << "\n";
	  */
	  
	  size_t hash_val = hasher(t) % NumBins;
	  size_t pos = hash_val / BinsPerSlot;
	  size_t offset_bits = (hash_val % BinsPerSlot) * BitsPerBin;
	  size_t target_bits = (_slots[pos] >> offset_bits) & MASK;

	  /*
	  std::cout << "(contains) checked bits at pos " << pos 
		    << " at bit offset " << offset_bits
		    << " using mask " << MASK
		    << " hashed as " << hash_val
		    << " as " << target_bits << "\n";
	  */

	  return (target_bits > 0);
        }
      };
    } // namespace detail
  } // namespace bloom_filter
} // namespace boost
#endif
