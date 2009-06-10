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

template <size_t Offset>
struct hash {
    template <class T>
    size_t operator() (T const & element) const {
        size_t seed = Offset;
        hash_combine(seed, element);
        return seed;
    }
};

int main(int argc, char * argv[]) {
    bloom_filter<int, tuple<hash<1>, hash<2>, hash<3> > > bf(2048);
    bf.insert(1);
    assert(bf.contains(1));
    return EXIT_SUCCESS;
}

