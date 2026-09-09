// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

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


#ifndef LUABIND_DECORATE_TYPE_HPP_INCLUDED
#define LUABIND_DECORATE_TYPE_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/primitives.hpp>
#include <luabind/detail/typetraits.hpp>

namespace luabind { namespace detail
{

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template<class T>
    struct decorated_type
    {
        static by_value<T> t;
        static inline by_value<T>& get() { return /*by_value<T>()*/t; }
    };

    template<class T>
    by_value<T> decorated_type<T>::t;

    template<class T>
    struct decorated_type<T*>
    {
        static by_pointer<T> t;
        static inline by_pointer<T>& get() { return /*by_pointer<T>()*/t; }
    };

    template<class T>
    by_pointer<T> decorated_type<T*>::t;

    template<class T>
    struct decorated_type<const T*>
    {
        static by_const_pointer<T> t;
        static inline by_const_pointer<T> get() { return /*by_const_pointer<T>()*/t; }
    };

    template<class T>
    by_const_pointer<T> decorated_type<const T*>::t;

    template<class T>
    struct decorated_type<const T* const>
    {
        static by_const_pointer<T> t;
        static inline by_const_pointer<T>& get() { return /*by_const_pointer<T>()*/t; }
    };

    template<class T>
    by_const_pointer<T> decorated_type<const T* const>::t;

    template<class T>
    struct decorated_type<T&>
    {
        static by_reference<T> t;
        static inline by_reference<T>& get() { return /*by_reference<T>()*/t; }
    };

    template<class T>
    by_reference<T> decorated_type<T&>::t;

    template<class T>
    struct decorated_type<const T&>
    {
        static by_const_reference<T> t;
        static inline by_const_reference<T>& get() { return /*by_const_reference<T>()*/t; }
    };

    template<class T>
    by_const_reference<T> decorated_type<const T&>::t;

#ifndef LUABIND_NO_RVALUE_REFERENCES
    template<class T>
    struct decorated_type<T&&>
    {
        static by_value<T> t;
        static inline by_value<T>& get() { return /*by_value<T>()*/t; }
    };

    template<class T>
    by_value<T> decorated_type<T&&>::t;
#endif

    #define LUABIND_DECORATE_TYPE(t) luabind::detail::decorated_type<t>::get()

#else

#include <boost/type_traits/is_array.hpp>

    namespace
    {
        LUABIND_ANONYMOUS_FIX char decorated_type_array[64];
    }

    template<class T>
    struct decorated_type_cref_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_const_reference<U> get(const U&)
        { return by_const_reference<U>(); }
        static T data() { return reinterpret_cast<T>(decorated_type_array); }
#else
        static void(*data())(T)
        { return (void(*)(T))0; }

        template<class U>
        static by_const_reference<U> get(void(*f)(const U&))
        { return by_const_reference<U>(); }
#endif
    };

    template<class T>
    struct decorated_type_ref_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_reference<U> get(U&)
        { return by_reference<U>(); }
        static T data() { return reinterpret_cast<T>(decorated_type_array); }
#else
        static void(*data())(T)
        { return (void(*)(T))0; }

        template<class U>
        static by_reference<U> get(void(*)(U&))
        { return by_reference<U>(); }
#endif
    };

    template<class T>
    struct decorated_type_cptr_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_const_pointer<U> get(const U*)
        { return by_const_pointer<U>(); }
        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
        template<class U>
        static by_const_pointer<U> get(const U*)
        { return by_const_pointer<U>(); }
        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#endif
    };

    template<class T>
    struct decorated_type_ptr_impl
    {
        template<class U>
        static by_pointer<U> get(U*)
        { return by_pointer<U>(); }
        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
    };

    template<class T>
    struct decorated_type_value_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_value<U> get(U&)
        { return by_value<U>(); }
        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
        static void(*data())(T&)
        { return (void(*)(T&))0; }

        template<class U>
        static by_value<U> get(void(*)(U&))
        { return by_value<U>(); }
#endif
    };

    template<>
    struct decorated_type_value_impl<void>
    {
        static by_value<void> get(int)
        {
            return by_value<void>();
        }
        static int data() { return 0; }
    };

    // Nested mpl::if_<> as a base class is unparseable on MSVC 6
    // (C2518 / "if_ is already a direct base"). Flatten via typedefs.
    // Const reference must be selected before value — otherwise
    // decorated_type_value_impl<T const&> forms T const& & (C2529).
    template<class T>
    struct decorated_type_select_cptr
    {
        typedef LUABIND_MSVC_TYPENAME boost::mpl::if_<
            luabind::detail::is_const_pointer<T>
          , decorated_type_cptr_impl<T>
          , decorated_type_value_impl<T>
        >::type type;
    };

    template<class T>
    struct decorated_type_select_ptr
    {
        typedef LUABIND_MSVC_TYPENAME boost::mpl::if_<
            luabind::detail::is_nonconst_pointer<T>
          , decorated_type_ptr_impl<T>
          , LUABIND_MSVC_TYPENAME decorated_type_select_cptr<T>::type
        >::type type;
    };

    template<class T>
    struct decorated_type_select_ref
    {
        typedef LUABIND_MSVC_TYPENAME boost::mpl::if_<
            luabind::detail::is_nonconst_reference<T>
          , decorated_type_ref_impl<T>
          , LUABIND_MSVC_TYPENAME decorated_type_select_ptr<T>::type
        >::type type;
    };

    template<class T>
    struct decorated_type_select_cref
    {
        typedef LUABIND_MSVC_TYPENAME boost::mpl::if_<
            luabind::detail::is_const_reference<T>
          , decorated_type_cref_impl<T>
          , LUABIND_MSVC_TYPENAME decorated_type_select_ref<T>::type
        >::type type;
    };

    template<class T>
    struct decorated_type
        : decorated_type_select_cref<T>::type
    {
    };

    #define LUABIND_DECORATE_TYPE(t) luabind::detail::decorated_type<t>::get(luabind::detail::decorated_type<t>::data())

#endif

}}

#endif // LUABIND_DECORATE_TYPE_HPP_INCLUDED
