// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/bloom_filter.hpp>
#include <boost/detail/lightweight_test.hpp>

using boost::bloom_filter;

int main(int argc, char * argv[]) {
    typedef bloom_filter<int> filter_type;
    filter_type filter(256);
    filter_type filter_copy = filter;
    assert(filter == filter_copy);
    filter_type other_filter(1024);
    assert(filter != other_filter);
    assert(filter_copy != other_filter);
    // assignment test
    filter_copy = other_filter;
    assert(filter_copy == other_filter);
    assert(filter_copy != filter);
    // swap test
    swap(filter, other_filter);
    assert(filter_copy != other_filter);
    assert(filter_copy == filter);
}

