#ifndef BLOOM_FILTER_20090608_0
#define BLOOM_FILTER_20090608_0

// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <bitset>
#include <boost/array.hpp>
#include <boost/function.hpp>

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/bloom_filter/detail/default_hash.hpp>

namespace boost {

    template <class Input, size_t M, size_t K>
    struct bloom_filter {
        private:
            std::bitset<M> bit_set;
            array<function<size_t(Input)>, K> hash_array;

            typedef typename add_reference<typename add_const<Input>::type>::type const_ref;

        public:
            typedef std::bitset<M> bitset_type;
            static size_t const size = M;
            static size_t const functions = K;
            typedef Input element_type;

            explicit bloom_filter(
                    array<function<size_t(Input)>, K> const & hash_functions
                    ) :
                hash_array(hash_functions) {}

            bloom_filter() {
                for(size_t k = 0; k < K; ++k)
                    hash_array[k] = detail::default_hash<Input,M>(k);
            }

            bloom_filter(bloom_filter const & other) :
                bit_set(other.bit_set), hash_array(other.hash_array) {}

            bloom_filter & operator=(bloom_filter rhs) {
                rhs.swap(*this);
                return *this;
            }

            bloom_filter & swap(bloom_filter & other) {
                using std::swap;
                swap(bit_set, other.bit_set);
                swap(hash_array, other.hash_array);
                return *this;
            }

            void insert(const_ref input) {
                for(size_t k = 0; k < K; ++k)
                    bit_set[hash_array[k](input)] = 1;
            }

            bool contains(const_ref input) const {
                bool result = true;
                for(size_t k = 0; k < K && result; ++k)
                    result = result && bit_set[hash_array[k](input)];
                return result;
            }

            bool operator[](const_ref input) const {
                return contains(input);
            }

            bloom_filter & clear() {
                bit_set.reset();
                return *this;
            }

            bitset_type const & state() const { 
                return bit_set; 
            }
    };
}

#endif

