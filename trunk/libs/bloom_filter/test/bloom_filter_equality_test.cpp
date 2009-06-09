// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/bloom_filter.hpp>
#include <boost/detail/lightweight_test.hpp>

using boost::bloom_filter;

int main(int argc, char * arg[]) {
    bloom_filter<int, 32, 3> filter1, filter2;
    assert(filter1 == filter2);
    filter1.insert(1);
    assert(filter1 != filter2);
    filter2.insert(1);
    assert(filter1 == filter2);
    return EXIT_SUCCESS;
}

