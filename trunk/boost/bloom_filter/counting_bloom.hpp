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

#ifndef BOOST_BLOOM_FILTER_COUNTING_BLOOM_HPP
#define BOOST_BLOOM_FILTER_COUNTING_BLOOM_HPP 1
/**
 * \author Alejandro Cabrera
 * \brief A generic counting Bloom filter providing compile-time unrolling
 *        of hash function application.
 */
#include <cmath>
#include <boost/array.hpp>

#include <boost/config.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>

#include <boost/bloom_filter/detail/apply_hash.hpp>
#include <boost/bloom_filter/hash/default.hpp>

#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
#include <initializer_list>
#endif 

namespace boost {
  namespace bloom_filter {
    template <typename T,
	      size_t NumBins,
	      class HashFunctions = mpl::vector<boost_hash<T, 3> >,
	      typename Block,
	      typename BitsPerBin>
    class counting_bloom_filter {
      typedef boost::array<NumBlocks, Block> bucket_type;

    public:
      typedef T value_type;
      typedef T key_type;

    public:
      counting_bloom_filter() {}

      template <typename InputIterator>
      counting_bloom_filter(const InputIterator start, const InputIterator end) {
	for (InputIterator i = start; i != end; ++i)
	  this->insert(*i);
      }

#ifndef BOOST_NO_0X_HDR_INITIALIZER_LIST
      counting_bloom_filter(const std::initializer_list<T>& ilist) {
	typedef typename std::initializer_list<T>::const_iterator citer;
	for (citer i = ilist.begin(), end = ilist.end(); i != end; ++i) {
	  this->insert(*i);
	}
      }
#endif

      static BOOST_CONSTEXPR size_t num_blocks() {
        return NumBlocks;
      }

      static BOOST_CONSTEXPR size_t bit_capacity() {
        return NumBlocks * sizeof(Block);
      }

      static BOOST_CONSTEXPR size_t num_hash_functions() {
        return mpl::size<HashFunctions>::value;
      };

      double false_positive_rate() const {
        const double n = static_cast<double>(this->count());
        static const double k = static_cast<double>(num_hash_functions());
        static const double m = static_cast<double>(Size);
        static const double e =
	  2.718281828459045235360287471352662497757247093699959574966;
        return std::pow(1 - std::pow(e, -k * n / m), k);
      };

      size_t count() const {
        return std::accumulate(this->blocks.begin(), this->blocks.end(), 0);
      };

      bool empty() const {
	return this->count() == 0;
      }

      void insert(const T& t) {
        static const unsigned N = mpl::size<HashFunctions>::value - 1;
        //detail::counting_apply_hash<N, T, Size, HashFunctions>::insert(t, bits);
      }

      template <typename InputIterator>
      void insert(const InputIterator start, const InputIterator end) {
	for (InputIterator i = start; i != end; ++i) {
	  this->insert(*i);
	}
      }

      void remove(const T& t);
      
      template <typename InputIterator>
      void insert(const InputIterator start, const InputIterator end) {
	for (InputIterator i = start; i != end; ++i) {
	  this->remove(*i);
	}
      }

      bool probably_contains(const T& t) const {
        static const unsigned N = mpl::size<HashFunctions>::value - 1;
        //return detail::apply_hash<N, T, Size, HashFunctions>::contains(t, bits);
	return false;
      }

      void clear() {
	for (bucket_type::const_iterator i = blocks.begin(), end = blocks.end();
	     i != end; ++i) {
	  *i = 0;
	}
      }

      void swap(counting_bloom_filter& other) {
	counting_bloom_filter tmp = other;
	other = *this;
	*this = tmp;
      }

      counting_bloom_filter& operator|=(const counting_bloom_filter& rhs);
      counting_bloom_filter& operator&=(const bloom_filter& rhs);

    private:
      bucket_type blocks;
    };

    template<class _T, size_t _Size, class _HashFunctions>
    bloom_filter<_T, _Size, _HashFunctions>
    operator|(const bloom_filter<_T, _Size, _HashFunctions>& lhs,
	      const bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      bloom_filter<_T, _Size, _HashFunctions> ret(lhs);
      ret |= rhs;
      return ret;
    }

    template<class _T, size_t _Size, class _HashFunctions>
    bloom_filter<_T, _Size, _HashFunctions>
    operator&(const bloom_filter<_T, _Size, _HashFunctions>& lhs,
	      const bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      bloom_filter<_T, _Size, _HashFunctions> ret(lhs);
      ret &= rhs;
      return ret;
    }

    template<class _T, size_t _Size, class _HashFunctions>
    void
    swap(bloom_filter<_T, _Size, _HashFunctions>& lhs,
	 bloom_filter<_T, _Size, _HashFunctions>& rhs)
    {
      lhs.swap(rhs);
    }
  } // namespace bloom_filter
} // namespace boost
#endif
