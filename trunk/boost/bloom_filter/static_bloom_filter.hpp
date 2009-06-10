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
#include <boost/bloom_filter/detail/default_hash.hpp>

namespace boost {

    template <class Input, size_t M, size_t K>
    struct static_bloom_filter {
        public:
            typedef std::bitset<M> bitset_type;

        private:
            bitset_type bit_set;
            array<function<size_t(Input)>, K> hash_array;

            typedef typename add_reference<typename add_const<Input>::type>::type const_ref;

        public:
            static size_t const size = M;
            static size_t const functions = K;
            typedef Input element_type;

            explicit static_bloom_filter(
                    array<function<size_t(Input)>, K> const & hash_functions
                    ) :
                hash_array(hash_functions) {}

            static_bloom_filter(bitset_type const & initial_state = bitset_type()) 
                : bit_set(initial_state)
            {
                for(size_t k = 0; k < K; ++k)
                    hash_array[k] = detail::default_hash<Input>(k);
            }

            static_bloom_filter(static_bloom_filter const & other) :
                bit_set(other.bit_set), hash_array(other.hash_array) {}

            static_bloom_filter & operator=(static_bloom_filter rhs) {
                rhs.swap(*this);
                return *this;
            }

            static_bloom_filter & swap(static_bloom_filter & other) {
                using std::swap;
                swap(bit_set, other.bit_set);
                swap(hash_array, other.hash_array);
                return *this;
            }

            void insert(const_ref input) {
                for(size_t k = 0; k < K; ++k)
                    bit_set[hash_array[k](input) % M] = 1;
            }

            bool contains(const_ref input) const {
                bool result = true;
                for(size_t k = 0; k < K && result; ++k)
                    result = result && bit_set[hash_array[k](input) % M];
                return result;
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
                // FIXME For some reason, we cannot compare boost::function instances...
                // return (hash_array == other.hash_array) && (bit_set == other.bit_set);
                return bit_set == other.bit_set;
            }

            bool operator!= (static_bloom_filter const & other) const {
                return !(*this == other);
            }
    };

    template <class Input, size_t M, size_t K>
        inline void swap(
                static_bloom_filter<Input, M, K> & left, 
                static_bloom_filter<Input, M, K> & right) {
            left.swap(right);
        }
}

#endif

