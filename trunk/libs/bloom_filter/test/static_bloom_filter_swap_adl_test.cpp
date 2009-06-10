// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/bloom_filter.hpp>
#include <boost/detail/lightweight_test.hpp>

using boost::static_bloom_filter;

int main(int argc, char * argv[]) {
    typedef static_bloom_filter<uint32_t, 32> filter_type;
    filter_type filter1, filter2;
    filter1.insert(1u);
    filter2.insert(2u);
    assert(!filter1.contains(2u));
    assert(filter1.contains(1u));
    assert(!filter2.contains(1u));
    assert(filter2.contains(2u));
    swap(filter1, filter2);
    assert(filter1.contains(2u));
    assert(!filter1.contains(1u));
    assert(filter2.contains(1u));
    assert(!filter2.contains(2u));
    return EXIT_SUCCESS;
}
