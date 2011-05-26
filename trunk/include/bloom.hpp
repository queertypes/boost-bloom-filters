#ifndef _BLOOM_HPP
#define _BLOOM_HPP 1
/**
 * \author Alejandro Cabrera
 * \brief A generic Bloom filter providing compile-time unrolling
 *        of hash function application.
 */
#include <boost/config.hpp>
#include <bitset>
#include <tuple>
#include "hash.hpp"

template <size_t N,
	  typename T,
	  size_t Size,
	  class HashFunctions>
struct apply_hash
{
  static void insert(const T& t, std::bitset<Size>& _bits) {
    typedef typename std::tuple_element<N, HashFunctions>::type Hash;
    _bits[Hash::hash(t) % Size] = true;
    apply_hash<N-1, T, Size, HashFunctions>::insert(t, _bits);
  }

  static bool contains(const T& t, const std::bitset<Size>& _bits) {
    typedef typename std::tuple_element<N, HashFunctions>::type Hash;
    return (_bits[Hash::hash(t) % Size] && 
	    apply_hash<N-1, T, Size, HashFunctions>::contains(t, _bits));
  }
};

/**
 * \todo: clean up tuples here, as well
 */
template <typename T,
	  size_t Size,
	  class HashFunctions>
struct apply_hash<0, T, Size, HashFunctions>
{    
  static void insert(const T& t, std::bitset<Size>& _bits) {
    typedef typename std::tuple_element<0, HashFunctions>::type Hash;
    _bits[Hash::hash(t) % Size] = true;
  }

  static bool contains(const T& t, const std::bitset<Size>& _bits) {
    typedef typename std::tuple_element<0, HashFunctions>::type Hash;
    return (_bits[Hash::hash(t) % Size]);
  }
};

/**
 * \todo: clean-up use of tuple here. Not all compilers support std::tuple
 */
template <typename T, 
	  size_t Size,
	  class HashFunctions = std::tuple<MurmurHash3<T, 3>, 
					   MurmurHash3<T, 5>,
					   MurmurHash3<T, 7> > >
class bloom_filter {
  typedef std::bitset<Size> Bitset;

public:
  bloom_filter() {}

  // \todo: need to add compiler check for constexpr
  BOOST_CONSTEXPR size_t size() const {
    return Size;
  }

  void insert(const T& t) {
    static const unsigned size = std::tuple_size<HashFunctions>::value - 1;
    apply_hash<size, T, Size, HashFunctions>::insert(t, bits);
  }

  bool contains(const T& t) const {
    static const unsigned size = std::tuple_size<HashFunctions>::value - 1;
    return apply_hash<size, T, Size, HashFunctions>::contains(t, bits);
  }

  bool operator[](const T& t) const {
    return this->contains(t);
  }

  void clear() {
    this->bits.reset();
  }

  // \todo: need to add compiler check for rvalue references
  bloom_filter(const bloom_filter&&);
  bloom_filter& operator=(const bloom_filter&& other);

  bloom_filter& operator|=(const bloom_filter& rhs) {
    this->bits |= rhs.bits;
  }

  bloom_filter& operator&=(const bloom_filter& rhs) {
    this->bits &= rhs.bits;
  }

  template<class _T, size_t _Size, class _HashFunctions> 
  friend bloom_filter<_T, _Size, _HashFunctions>& 
  operator|(const bloom_filter<_T, _Size, _HashFunctions>& lhs,
	    const bloom_filter<_T, _Size, _HashFunctions>& rhs)
  {
    bloom_filter<_T, _Size, _HashFunctions> ret = lhs;
    return (ret |= rhs);
  }

  template<class _T, size_t _Size, class _HashFunctions> 
  friend bloom_filter<_T, _Size, _HashFunctions>& 
  operator&(const bloom_filter<_T, _Size, _HashFunctions>& lhs,
	    const bloom_filter<_T, _Size, _HashFunctions>& rhs)
  {
    bloom_filter<_T, _Size, _HashFunctions> ret = lhs;
    return (ret &= rhs);
  }

private:
  std::bitset<Size> bits;
};
#endif
