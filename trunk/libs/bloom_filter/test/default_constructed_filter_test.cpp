// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/bloom_filter/bloom_filter.hpp>
#include <boost/detail/lightweight_test.hpp>

#define FILTER_SIZE 256

using boost::bloom_filter;

int main(int argc, char * argv[]) {
    typedef bloom_filter<uint32_t, FILTER_SIZE, 3> filter_type;
    filter_type filter; // default constructed
    filter.insert(1u);
    assert(filter.contains(1u));
    return EXIT_SUCCESS;
}

