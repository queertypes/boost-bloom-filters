#ifndef BOOST_BLOOM_FILTER_DETAIL_DEFAULT_HASH_20090609_0
#define BOOST_BLOOM_FILTER_DETAIL_DEFAULT_HASH_20090609_0

#include <string>
#include <boost/crc.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost {

    namespace detail {

        template <class Input, size_t M>
            struct default_hash {
                typedef typename add_reference<typename add_const<Input>::type>::type const_ref;

                size_t offset_;
                explicit default_hash(size_t offset)
                    : offset_(offset) {}

                size_t operator()(std::string const & input) const {
                    crc_32_type crc32_computer;
                    crc32_computer.process_bytes(input.c_str(), input.size());
                    return (crc32_computer.checksum() + offset_) % M;
                }

                size_t operator()(const_ref input) const {
                    crc_32_type crc32_computer;
                    crc32_computer.process_bytes(&input, sizeof(input));
                    return (crc32_computer.checksum() + offset_) % M;
                }
            };

    } // namespace detail

} // namespace boost

#endif

