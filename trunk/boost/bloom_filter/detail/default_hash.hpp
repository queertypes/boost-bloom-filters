#ifndef BOOST_BLOOM_FILTER_DETAIL_DEFAULT_HASH_20090609_0
#define BOOST_BLOOM_FILTER_DETAIL_DEFAULT_HASH_20090609_0

#include <string>
#include <boost/functional/hash.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost {

    namespace bloom_filters {

        namespace detail {

            template <size_t Seed = 0>
                struct default_hash {
                    template <class T>
                    size_t operator() (T const & input) const {
                        size_t seed = Seed;
                        hash_combine(seed, input);
                        return seed;
                    }
                };

        } // namespace detail

    } // namespace bloom_filters

} // namespace boost

#endif

