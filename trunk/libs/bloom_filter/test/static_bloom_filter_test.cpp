// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/bloom_filter.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/fusion/tuple.hpp>

#define FILTER_SIZE 256

struct hash1 {
    size_t operator()(uint32_t id) const {
        return ((id << 4) | (id >> 4)) % FILTER_SIZE;
    }
};

struct hash2 {
    size_t operator()(uint32_t id) const {
        return (id * id) % FILTER_SIZE;
    }
};

struct hash3 {
    size_t operator()(uint32_t id) const {
        return (id * 97) % FILTER_SIZE;
    }
};

using std::bitset;
using std::cout;
using std::endl;
using boost::function;
using boost::static_bloom_filter;
using boost::fusion::tuple;
using boost::fusion::make_tuple;

int main(int argc, char * argv[]) {
    typedef static_bloom_filter<uint32_t, FILTER_SIZE, tuple<hash1, hash2, hash3> > filter_type;
    filter_type filter(make_tuple(hash1(), hash2(), hash3()));
    filter_type filter_copy = filter;
    for(uint32_t i = 0; i < 10; ++i) filter.insert(i);
    for(uint32_t i = 0; i < 10; ++i) assert(filter.contains(i));
    filter_type::bitset_type bit_set = filter.state();
    cout << endl;
    // assignment test
    filter_copy = filter;
    return EXIT_SUCCESS;
}

