#ifndef BLOOM_FILTER_INTERNALS_20090610_0
#define BLOOM_FILTER_INTERNALS_20090610_0

// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost {

    namespace bloom_filters {

        namespace detail {

            template <class Input, class BitSet>
                class internals {
                    protected:
                        typedef BitSet bitset_type;
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

                };

        } // namespace detail

    } // namespace bloom_filters

} // namespace boost

#endif
