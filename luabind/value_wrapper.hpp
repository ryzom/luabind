// Copyright (c) 2005 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef LUABIND_VALUE_WRAPPER_050419_HPP
#define LUABIND_VALUE_WRAPPER_050419_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace luabind {

//
// Concept ``ValueWrapper``
//

template<class T>
struct value_wrapper_traits
{
    typedef boost::mpl::false_ is_specialized;
};

template<class T>
struct is_value_wrapper
  : value_wrapper_traits<T>::is_specialized
{};

} // namespace luabind

namespace luabind {

template<class T>
struct is_value_wrapper_arg
  : is_value_wrapper<
      typename boost::remove_const<
          typename boost::remove_reference<T>::type
      >::type
    >
{};

} // namespace luabind

#endif // LUABIND_VALUE_WRAPPER_050419_HPP
