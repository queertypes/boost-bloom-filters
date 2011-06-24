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

#ifndef BOOST_BLOOM_FILTER_DYNAMIC_BLOOM_HPP
#define BOOST_BLOOM_FILTER_DYNAMIC_BLOOM_HPP 1
/**
 * \author Alejandro Cabrera
 * \brief A generic Bloom filter providing compile-time unrolling
 *        of hash function application. 
 */
#include <cmath>

#include <boost/config.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/dynamic_bitset.hpp>

#include <boost/bloom_filter/detail/apply_hash.hpp>
#include <boost/bloom_filter/hash/default.hpp>

#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
#include <initializer_list>
#endif 

namespace boost {
  namespace bloom_filter {
    template <typename T,
	      class HashFunctions = mpl::vector<boost_hash<T, 3> >,
	      class Block = size_t,
	      class Allocator = std::allocator<Block> >
    class dynamic_bloom_filter {
    public:
      typedef T value_type;
      typedef T key_type;
      typedef HashFunctions hash_function_type;
      typedef Block block_type;
      typedef Allocator allocator_type;

    public:
      
      // constructors
      dynamic_bloom_filter() {}
      
      explicit dynamic_bloom_filter(const size_t bit_capacity);

      template <typename InputIterator>
      dynamic_bloom_filter(const InputIterator start, 
			   const InputIterator end) {
	for (InputIterator i = start; i != end; ++i)
	  this->insert(*i);
      }

#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
      dynamic_bloom_filter(const std::initializer_list<T>& ilist) {
	typedef typename std::initializer_list<T>::const_iterator citer;
	for (citer i = ilist.begin(), end = ilist.end(); i != end; ++i) {
	  this->insert(*i);
	}
      }
#endif

      // query functions
      static BOOST_CONSTEXPR size_t num_hash_functions() {
        return mpl::size<HashFunctions>::value;
      };

      double false_positive_rate() const {
        const double n = static_cast<double>(this->bits.count());
        static const double k = static_cast<double>(num_hash_functions());
        static const double m = static_cast<double>(Size);
        static const double e =
	  2.718281828459045235360287471352662497757247093699959574966;
        return std::pow(1 - std::pow(e, -k * n / m), k);
      };

      size_t count() const {
        return this->bits.count();
      };

      bool empty() const {
	return this->count() == 0;
      }

      // core operations
      void insert(const T& t) {
        static const unsigned N = mpl::size<HashFunctions>::value - 1;
        //detail::apply_hash<N, T, Size, HashFunctions>::insert(t, bits);
      }

      template <typename InputIterator>
      void insert(const InputIterator start, const InputIterator end) {
	for (InputIterator i = start; i != end; ++i) {
	  this->insert(*i);
	}
      }

      bool probably_contains(const T& t) const {
        static const unsigned N = mpl::size<HashFunctions>::value - 1;
        //return detail::apply_hash<N, T, Size, HashFunctions>::contains(t, bits);
	return false;
      }

      // auxilliary operations
      void clear() {
        this->bits.reset();
      }

      void swap(bloom_filter& other) {
	bloom_filter tmp = other;
	other = *this;
	*this = tmp;
      }

      void resize(const size_t bit_capacity);

      bloom_filter& operator|=(const bloom_filter& rhs) {
        this->bits |= rhs.bits;
        return *this;
      }

      bloom_filter& operator&=(const bloom_filter& rhs) {
        this->bits &= rhs.bits;
        return *this;
      }

    private:
      dynamic_bitset<block_type, allocator_type> bits;
    };

    template<class T, class HashFunctions,
	     class Block, class Allocator>
    dynamic_bloom_filter<T, HashFunctions, Block, Allocator>
    operator|(const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& lhs,
	      const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& rhs)
    {
      bloom_filter<_T, _Size, _HashFunctions> ret(lhs);
      ret |= rhs;
      return ret;
    }

    template<class T, class HashFunctions,
	     class Block, class Allocator>
    dynamic_bloom_filter<T, HashFunctions, Block, Allocator>
    operator&(const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& lhs,
	      const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& rhs)
    {
      bloom_filter<_T, _Size, _HashFunctions> ret(lhs);
      ret &= rhs;
      return ret;
    }

    template<class T, class HashFunctions,
	     class Block, class Allocator>
    void
    swap(dynamic_bloom_filter<T, 
			      HashFunctions, 
			      Block, Allocator>& lhs,
	 dynamic_bloom_filter<T, 
			      HashFunctions, 
			      Block, Allocator>& rhs)
    {
      lhs.swap(rhs);
    }
  } // namespace bloom_filter
} // namespace boost
#endif
