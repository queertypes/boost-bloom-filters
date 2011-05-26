:Authors:
    Dean Michael Berris <mikhailberis@gmail.com>

:Version:
    0.1 of 2009/06/08

:License:
    Distributed under the Boost Software License
    (See accompanying LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

==================
Boost.Bloom_filter
==================

From http://en.wikipedia.org/wiki/Bloom_filter ::

    A Bloom Filter is a space-efficient probabilistic data structure 
    that is used to test whether an element is a member of a set. False 
    positives are possible but false negatives are not. Elements can be 
    added to the set, but not removed. The more that are added to the 
    set, the larger the probability of false positives.

Boost.Bloom_filter is a generic implementation of a Bloom Filter that uses
a variable number of bits to represent a generic bloom filter that takes a
configurable input type and a configurable number of hash functions. Bloom
Filters work by using multiple hash functions to determine the membership
of an element in the represented set.

For instance, we can use a Bloom Filter to maintain a set of URLs that have
been cached by a caching web proxy. Instead of storing every possible URL,
we use a Bloom Filter to mark certain parts of a bit field as '1' for every
location that a set of hash functions returns. As an example, consider the
empty 32-bit bitset representing a Bloom Filter below::

    00000000000000000000000000000000

From here, we then have for instance three (3) hash functions that when we
feed a URL will return numbers from [0..31]. Let's say when we hash the
string 'http://www.boost.org' we get the three indexes ``(0,9,15)``. Our
bitset will then look like::

    10000000010000010000000000000000

To find out whether we've encountered a certain URL before, we simply pass
the URL to the same three hash functions. If we see that all the positions
returned by the three hash functions are all ``1``, then we might have seen
the URL before -- this is called a false positive. In the case of a caching
web proxy this shouldn't be a problem because if it really hasn't gotten it
yet, it's just going to fetch it again accordingly (and update the Bloom
Filter after that). This matters when you have a hierarchical network of
web cache proxies that need to know where a page may have been cached before.
Sharing these bloom filters across the nodes of the network allows for
optimizing for the case when you may have already cached a page before.

This also applies when writing distributed filesystems where you may have a
system of multi-layer caches. Sharing the bloom filters of the pages that may
have already been accessed (read) by nearby storage/retrieval nodes that have
these pages cached minimizes the number of times a page that has already been
retrieved before will have to be retrieved again across the network. In these
situations you may be able to tolerate false positives but would rather not
have false negatives -- meaning data you haven't ever accessed before would
not be available in the caches and therefore will need to be fetched again.

Synopsis
--------

A `bloom_filter` is a template class which takes three parameters:

Input
    The type of the elements contained in a bloom filter.
M
    The number of bits contained by the internal bitset.
K
    The number of hash functions used by the bloom filter.

To use a `bloom_filter` that maintains a set of integers using 256 bits and
two hash functions, we could have the following code listing::

    #include <boost/bloom_filter.hpp>

    ...

    using boost::bloom_filter;
    using boost::array;
    using boost::function;

    ...

    array<function<size_t(int)>, 2> hash_array;
    hash_array[0] = hash1;
    hash_array[1] = hash2;
    bloom_filter<int, 256, 2> integers(hash_array);

Here we first create a Boost.Array of two Boost.Functions that we then use
to construct the `bloom_filter`. In this code listing we are then able to
insert new integers into the set by simply doing::

    integers.insert(1);
    integers.insert(7);
    integers.insert(5);

We are also then able to check whether certain integers are part of the set.
Remember that in Bloom Filters, there is a possibility of getting false 
positives::

    integers.contains(1); // should be true
    integers.contains(9); // may be true or false
    integers[5]; // should be true

Reference
---------

* Bloom Filter (Wikipedia Entry) http://en.wikipedia.org/wiki/Bloom_filter


