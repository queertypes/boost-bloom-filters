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

namespace boost {

    template <class Input, class Sequence, class Block = unsigned char, class Allocator = std::allocator<unsigned char> >
        struct bloom_filter {
            public:
                typedef dynamic_bitset<Block, Allocator> bitset_type;

            private:
                bitset_type bit_set;
                Sequence hash_functions;

                typedef typename add_reference<typename add_const<Input>::type>::type const_ref;

                struct insert_impl {
                    bitset_type & bit_set_;
                    const_ref input_;
                    insert_impl(bitset_type & bit_set, const_ref input)
                        : bit_set_(bit_set), input_(input)
                    {}
                    template <class F>
                        void operator()(F const & f) const {
                            bit_set_[f(input_) % bit_set_.size()] = true;
                        }
                };

                struct contains_impl {
                    bitset_type const & bit_set_;
                    const_ref input_;
                    bool & result_;
                    contains_impl(bitset_type const & bit_set, const_ref input, bool & result)
                        : bit_set_(bit_set), input_(input), result_(result)
                    {}
                    template <class F>
                        void operator()(F const & f) const {
                            result_ = result_ && bit_set_[f(input_) % bit_set_.size()];
                        }
                };

            public:
                bloom_filter(
                        size_t size, 
                        Sequence const & functions = Sequence())
                    : bit_set(size, 0), hash_functions(functions)
                { }

                void insert(const_ref input) {
                    using fusion::for_each;
                    for_each(
                            hash_functions, 
                            insert_impl(bit_set, input));
                }

                bool contains(const_ref input) const {
                    using fusion::for_each;
                    bool found = true;
                    for_each(
                            hash_functions, 
                            contains_impl(bit_set, input, found));
                    return found;
                }
        };

} // namespace boost

#endif

