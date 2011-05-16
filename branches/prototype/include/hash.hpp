#ifndef _HASH_HPP_
#define _HASH_HPP_ 1
/**
 * \author Alejandro Cabrera
 * \brief An alternative implentation of murmurhash3 for users
 *        not wishing to rely on the public domain Murmurhash3.
 * \todo Hash many more collisions than public domain version of murmurhash3.
 * \todo Provide 64-bit implementation of murmurhash3.
 */
#include <cstdint>

template <typename UnsignedIntT>
inline UnsignedIntT rotl(const UnsignedIntT x, uint8_t r)
{
  return (x << r) | (x >> (sizeof(UnsignedIntT) * 4 - r));
}

inline uint32_t rotl32(const uint32_t x, uint8_t r)
{
  return rotl<uint32_t>(x,r);
}

inline uint64_t rotl64(const uint64_t x, uint8_t r)
{
  return rotl<uint64_t>(x,r);
}

template <typename UnsignedIntT>
inline UnsignedIntT get_block(const UnsignedIntT * p, const int i)
{
  return p[i];
}

inline uint32_t fmix(uint32_t h)
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

void murmurhash3(const void *const key, const int len,
		 const uint32_t seed, void *out)
{
  static const uint32_t c1 = 0xcc9e2d51;
  static const uint32_t c2 = 0x1b873593;

  const uint8_t *const  data = static_cast<const uint8_t *const>(key);
  
  uint32_t h1 = seed;
  const uint32_t *const blocks = 
    reinterpret_cast<const uint32_t *const >(data + len);

  for (int i = -(len/4); i; ++i) {
    uint32_t k1 = blocks[i];
    
    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = rotl32(h1, 13);
    h1 = h1*5 + 0xe6546b64;
  }
  
  const uint8_t *const tail = 
    static_cast<const uint8_t *const>(data + len);

  uint32_t k1 = 0;

  switch (len & 3) {
  case 3: 
    k1 ^= tail[2] << 16;
  case 2: 
    k1 ^= tail[1] << 8;
  case 1: 
    k1 ^= tail[0];
    k1 *= c1;
    k1 = rotl32(k1, 16);
    k1 *= c2;
    h1 ^= k1;
  }

  h1 ^= len;
  h1 = fmix(h1);
  
  *static_cast<uint32_t *>(out) = h1;
}

template <typename T, size_t Seed>
struct MurmurHash3 {
  static size_t hash(const T& t) {
    size_t out = 0;
    murmurhash3(static_cast<const void *const>(&t), 
		sizeof(t),
		Seed,
		&out);
    return out;
  }
};
#endif
