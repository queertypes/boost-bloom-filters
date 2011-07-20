//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Alejandro Cabrera 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bloom_filter for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_BLOOM_FILTER_DETAIL_EXCEPTIONS_HPP
#define BOOST_BLOOM_FILTER_DETAIL_EXCEPTIONS_HPP

#include <exception>

namespace boost {
  namespace bloom_filters {
    namespace detail {

      class underflow : public std::exception {
	virtual const char *
	what() const throw() {
	  return "boost::bloom_filters::detail::underflow"; 
	}
      };

      class overrflow : public std::exception {
	virtual const char *
	what() const throw() {
	  return "boost::bloom_filters::detail::overflow"; 
	}
      };

    } // namespace detail
  } // namespace bloom_filter
} // namespace boost
#endif
