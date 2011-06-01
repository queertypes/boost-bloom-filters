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
#ifndef BOOST_BLOOM_FILTER_BLOOM_HPP
#define BOOST_BLOOM_FILTER_BLOOM_HPP 1
/**
 * \author Alejandro Cabrera
 * \brief A generic Bloom filter providing compile-time unrolling
 *        of hash function application.
 */
#include <bitset>

#include <boost/config.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>

#include <boost/bloom_filter/detail/apply_hash.hpp>
#include <boost/bloom_filter/hash.hpp>

namespace boost {
  template <typename T, 
	    size_t Size,
	    class HashFunctions = mpl::vector<boost::BoostHash<T, 3>, 
					      boost::BoostHash<T, 5>,
					      boost::BoostHash<T, 7> > >
  class bloom_filter {
  public:
    bloom_filter() {}

    BOOST_CONSTEXPR size_t size() const {
      return Size;
    }
    
    void insert(const T& t) {
      static const unsigned N = mpl::size<HashFunctions>::value - 1;
      detail::apply_hash<N, T, Size, HashFunctions>::insert(t, bits);
    }

    bool contains(const T& t) const {
      static const unsigned N = mpl::size<HashFunctions>::value - 1;
      return detail::apply_hash<N, T, Size, HashFunctions>::contains(t, bits);
    }
    
    void clear() {
      this->bits.reset();
    }
    
    bloom_filter& operator|=(const bloom_filter& rhs) {
      this->bits |= rhs.bits;
      return *this;
    }
  
    bloom_filter& operator&=(const bloom_filter& rhs) {
      this->bits &= rhs.bits;
      return *this;
    }

  private:
    std::bitset<Size> bits;
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
}
#endif
