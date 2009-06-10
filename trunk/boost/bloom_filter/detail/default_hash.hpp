#ifndef BOOST_BLOOM_FILTER_DETAIL_DEFAULT_HASH_20090609_0
#define BOOST_BLOOM_FILTER_DETAIL_DEFAULT_HASH_20090609_0

#include <string>
#include <boost/functional/hash.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost {

    namespace detail {

        template <class Input>
            struct default_hash {
                typedef typename add_reference<typename add_const<Input>::type>::type const_ref;
                size_t seed_;
                default_hash(size_t seed) 
                    : seed_(seed) {}

                size_t operator()(const_ref input) const {
                    size_t seed = seed_;
                    hash_combine(seed, input);
                    return seed;
                }
            };

    } // namespace detail

} // namespace boost

#endif

