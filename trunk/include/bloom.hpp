#ifndef _BLOOM_HPP
#define _BLOOM_HPP 1
/**
 * \author Alejandro Cabrera
 * \brief A generic Bloom filter providing compile-time unrolling
 *        of hash function application.
 */
#include <boost/config.hpp>
#include <bitset>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#include <hash.hpp>

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

template <typename T, 
	  size_t Size,
	  class HashFunctions = boost::mpl::vector<MurmurHash3<T, 3>, 
						   MurmurHash3<T, 5>,
						   MurmurHash3<T, 7> > >
class bloom_filter {
  typedef std::bitset<Size> Bitset;

public:
  bloom_filter() {}

  BOOST_CONSTEXPR size_t size() const {
    return Size;
  }

  void insert(const T& t) {
    static const unsigned N = boost::mpl::size<HashFunctions>::value - 1;
    apply_hash<N, T, Size, HashFunctions>::insert(t, bits);
  }

  bool contains(const T& t) const {
    static const unsigned N = boost::mpl::size<HashFunctions>::value - 1;
    return apply_hash<N, T, Size, HashFunctions>::contains(t, bits);
  }

  bool operator[](const T& t) const {
    return this->contains(t);
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
  bloom_filter<_T, _Size, _HashFunctions> ret = lhs;
  ret |= rhs;
  return ret;
}

template<class _T, size_t _Size, class _HashFunctions> 
bloom_filter<_T, _Size, _HashFunctions>
operator&(const bloom_filter<_T, _Size, _HashFunctions>& lhs,
	  const bloom_filter<_T, _Size, _HashFunctions>& rhs)
{
  bloom_filter<_T, _Size, _HashFunctions> ret = lhs;
  ret &= rhs;
  return ret;
}
#endif
