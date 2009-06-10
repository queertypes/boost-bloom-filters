// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/bloom_filter.hpp>
#include <boost/detail/lightweight_test.hpp>

using boost::bloom_filter;

int main(int argc, char * argv[]) {
    bloom_filter<int> filter1(256), filter2(256);
    assert(filter1 == filter2);
    filter1.insert(1);
    assert(filter1 != filter2);
    filter2.insert(1);
    assert(filter1 == filter2);
    return EXIT_SUCCESS;
}

