#ifndef _BLOOM_HPP
#define _BLOOM_HPP 1

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

template <typename T, 
	  size_t Size,
	  class HashFunctions = std::tuple<MurmurHash3<T, 3>, 
					   MurmurHash3<T, 5>,
					   MurmurHash3<T, 7> > >
class bloom_filter {
  typedef std::bitset<Size> Bitset;

public:
  bloom_filter() 
  {
  }

  constexpr size_t size() const {
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
    bits.reset();
  }

private:
  std::bitset<Size> bits;
};
#endif
