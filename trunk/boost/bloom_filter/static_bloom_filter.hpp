#ifndef STATIC_BLOOM_FILTER_20090608_0
#define STATIC_BLOOM_FILTER_20090608_0

// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <bitset>
#include <boost/array.hpp>
#include <boost/function.hpp>

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/bloom_filter/detail/default_hash.hpp>
#include <boost/bloom_filter/detail/internals.hpp>

namespace boost {

    namespace bloom_filters {

        template <
            class Input, 
            size_t M, 
            class HashFunctions = fusion::vector<
                detail::default_hash<0>,
                detail::default_hash<1>,
                detail::default_hash<2>
                >
            >
        struct static_bloom_filter : protected detail::internals<Input, std::bitset<M> > {
            public:
                typedef std::bitset<M> bitset_type;

            private:
                bitset_type bit_set;
                HashFunctions hash_functions;

                typedef typename add_reference<typename add_const<Input>::type>::type const_ref;
                typedef detail::internals<Input, std::bitset<M> > base;

                struct insert_impl {
                    bitset_type & bit_set_;
                };

            public:
                static size_t const size = M;
                typedef Input element_type;

                static_bloom_filter(
                        bitset_type const & initial_state = bitset_type(),
                        HashFunctions const & hash_functions = HashFunctions()) 
                    : bit_set(initial_state), hash_functions(hash_functions)
                {}

                explicit static_bloom_filter(
                        HashFunctions const & hash_functions
                        )
                    : bit_set(), hash_functions(hash_functions)
                {}

                static_bloom_filter(static_bloom_filter const & other) :
                    bit_set(other.bit_set), hash_functions(other.hash_functions) {}

                static_bloom_filter & operator=(static_bloom_filter rhs) {
                    rhs.swap(*this);
                    return *this;
                }

                static_bloom_filter & swap(static_bloom_filter & other) {
                    using std::swap;
                    swap(bit_set, other.bit_set);
                    swap(hash_functions, other.hash_functions);
                    return *this;
                }

                void insert(const_ref input) {
                    using fusion::for_each;
                    typedef typename base::insert_impl inserter;
                    for_each(
                            hash_functions,
                            inserter(bit_set, input)
                            );
                }

                bool contains(const_ref input) const {
                    using fusion::for_each;
                    typedef typename base::contains_impl contains_checker;
                    bool found = true;
                    for_each(
                            hash_functions,
                            contains_checker(bit_set, input, found)
                            );
                    return found;
                }

                bool operator[](const_ref input) const {
                    return contains(input);
                }

                static_bloom_filter & clear() {
                    bit_set.reset();
                    return *this;
                }

                bitset_type const & state() const { 
                    return bit_set; 
                }

                bool operator== (static_bloom_filter const & other) const {
                    return bit_set == other.bit_set;
                }

                bool operator!= (static_bloom_filter const & other) const {
                    return !(*this == other);
                }
        };

        template <class Input, size_t M, class HashFunctions>
            inline void swap(
                    static_bloom_filter<Input, M, HashFunctions> & left, 
                    static_bloom_filter<Input, M, HashFunctions> & right) {
                left.swap(right);
            }

    } // namespce bloom_filters

    using bloom_filters::static_bloom_filter;

} // namespace boost

#endif

