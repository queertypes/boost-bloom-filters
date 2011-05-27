#ifndef __APPLY_HASH_HPP
#define __APPLY_HASH_HPP

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
	_bits[Hash::hash(t) % Size] = true;
	apply_hash<N-1, T, Size, HashFunctions>::insert(t, _bits);
      }
    
      static bool contains(const T& t, const std::bitset<Size>& _bits) {
	typedef typename boost::mpl::at_c<HashFunctions, N>::type Hash;
	return (_bits[Hash::hash(t) % Size] && 
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
	_bits[Hash::hash(t) % Size] = true;
      }
    
      static bool contains(const T& t, const std::bitset<Size>& _bits) {
	typedef typename boost::mpl::at_c<HashFunctions, 0>::type Hash;
	return (_bits[Hash::hash(t) % Size]);
      }
    };
  }
}
#endif
