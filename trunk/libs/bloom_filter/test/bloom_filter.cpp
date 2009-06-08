// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <bitset>
#include <cassert>
#include <iostream>
#include "bloom_filter.hpp"

#define FILTER_SIZE 256

size_t hash1(uint32_t id) {
    return ((id << 4) | (id >> 4)) % FILTER_SIZE;
}

size_t hash2(uint32_t id) {
    return (id * id) % FILTER_SIZE;
}

size_t hash3(uint32_t id) {
    return (id * 97) % FILTER_SIZE;
}

using std::bitset;
using std::cout;
using std::endl;
using boost::array;
using boost::function;
using boost::bloom_filter;

int main(int argc, char * argv[]) {
    array<function<size_t(uint32_t)>, 3> functions;
    functions[0] = hash1;
    functions[1] = hash2;
    functions[2] = hash3;
    typedef bloom_filter<uint32_t, FILTER_SIZE, 3> filter_type;
    filter_type filter(functions);
    filter_type filter_copy = filter;
    for(uint32_t i = 0; i < 10; ++i) filter.insert(i);
    for(uint32_t i = 0; i < 10; ++i) assert(filter.contains(i));
    filter_type::bitset_type bit_set = filter.state();
    for(uint32_t i = 0; i < filter_type::size ; ++i)
        cout << (bit_set[i] ? '1' : '0');
    cout << endl;
    // assignment test
    filter_copy = filter;
    return EXIT_SUCCESS;
}

