#ifndef BLOOM_FILTER_20090610_0
#define BLOOM_FILTER_20090610_0

// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dynamic_bitset.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/bloom_filter/detail/internals.hpp>

namespace boost {

    template <
        class Input, 
        class Sequence = fusion::vector<
            detail::default_hash<0>,
            detail::default_hash<1>,
            detail::default_hash<2>
                >,
        class Block = unsigned char, 
        class Allocator = std::allocator<unsigned char> 
             >
        struct bloom_filter : protected detail::bloom_filter_internals<Input, dynamic_bitset<Block,Allocator> > {
            public:
                typedef dynamic_bitset<Block, Allocator> bitset_type;

            private:
                bitset_type bit_set;
                Sequence hash_functions;

                typedef typename add_reference<typename add_const<Input>::type>::type const_ref;
                typedef detail::bloom_filter_internals<Input, dynamic_bitset<Block,Allocator> > base;

            public:
                bloom_filter(
                        size_t size, 
                        Sequence const & functions = Sequence())
                    : bit_set(size, 0), hash_functions(functions)
                { }

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
        };

} // namespace boost

#endif

