// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/bloom_filter.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/functional/hash.hpp>

using boost::bloom_filter;
using boost::fusion::tuple;
using boost::hash_combine;

int main(int argc, char * argv[]) {
    bloom_filter<int> bf(2048);
    bf.insert(1);
    assert(bf.contains(1));
    assert(!bf.contains(2));
    return EXIT_SUCCESS;
}

