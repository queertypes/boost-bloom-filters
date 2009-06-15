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

    namespace bloom_filters {

        template <
            class Input, 
            class HashFunctions = fusion::vector<
                detail::default_hash<0>,
                detail::default_hash<1>,
                detail::default_hash<2>
                    >,
            class Block = unsigned char, 
            class Allocator = std::allocator<unsigned char> 
                 >
            struct bloom_filter : protected detail::internals<Input, dynamic_bitset<Block,Allocator> > {
                public:
                    typedef dynamic_bitset<Block, Allocator> bitset_type;

                private:
                    bitset_type bit_set;
                    HashFunctions hash_functions;

                    typedef typename add_reference<typename add_const<Input>::type>::type const_ref;
                    typedef detail::internals<Input, dynamic_bitset<Block,Allocator> > base;

                public:
                    bloom_filter(
                            size_t size, 
                            HashFunctions const & functions = HashFunctions())
                        : bit_set(size, 0), hash_functions(functions)
                    {}

                    bloom_filter(bloom_filter const & other)
                        : bit_set(other.bit_set), hash_functions(other.hash_functions)
                    {}

                    bloom_filter & operator= (bloom_filter rhs) {
                        return rhs.swap(*this);
                    }

                    bloom_filter & swap(bloom_filter & other) {
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

                    bool operator==(bloom_filter const & other) const {
                        return bit_set == other.bit_set;
                    }

                    bool operator!=(bloom_filter const & other) const {
                        return !(*this == other);
                    }
            };

        template <class Input, class HashFunctions, class Block, class Allocator>
            inline void swap(
                    bloom_filter<Input, HashFunctions, Block, Allocator> & left,
                    bloom_filter<Input, HashFunctions, Block, Allocator> & right
                    ) {
                left.swap(right);
            }

    } // namespace bloom_filters

    using bloom_filters::bloom_filter;

} // namespace boost

#endif

