// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

# ifndef LUABIND_DEDUCE_SIGNATURE_080911_HPP
#  define LUABIND_DEDUCE_SIGNATURE_080911_HPP

#  include <luabind/config.hpp>
#  include <luabind/detail/most_derived.hpp>

#  ifdef LUABIND_NO_BOOST_FUNCTION_TYPES

#   if LUABIND_MAX_ARITY <= 8
#    include <boost/mpl/vector/vector10.hpp>
#   else
#    include <boost/mpl/vector/vector50.hpp>
#   endif
#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/iteration/iterate.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>

namespace luabind { namespace detail {

namespace mpl = boost::mpl;

template <class R>
mpl::vector1<R> deduce_signature(R(*)(), ...)
{
    return mpl::vector1<R>();
}

template <class R, class T>
mpl::vector2<R,T&> deduce_signature(R(T::*)())
{
    return mpl::vector2<R,T&>();
}

template <class R, class T, class Wrapped>
mpl::vector2<R,typename most_derived<T,Wrapped>::type&>
deduce_signature(R(T::*)(), Wrapped*)
{
    return mpl::vector2<R,typename most_derived<T,Wrapped>::type&>();
}

template <class R, class T>
mpl::vector2<R,T const&> deduce_signature(R(T::*)() const)
{
    return mpl::vector2<R,T const&>();
}

template <class R, class T, class Wrapped>
mpl::vector2<R,typename most_derived<T,Wrapped>::type const&>
deduce_signature(R(T::*)() const, Wrapped*)
{
    return mpl::vector2<R,typename most_derived<T,Wrapped>::type const&>();
}

#   define BOOST_PP_ITERATION_PARAMS_1 \
     (3, (1, LUABIND_MAX_ARITY, <luabind/detail/deduce_signature.hpp>))
#   include BOOST_PP_ITERATE()

}} // namespace luabind::detail

#  else // LUABIND_NO_BOOST_FUNCTION_TYPES

#  include <boost/function_types/components.hpp>
#  include <boost/function_types/is_callable_builtin.hpp>
#  include <boost/function_types/is_member_function_pointer.hpp>
#  include <boost/type_traits/add_reference.hpp>
#  include <boost/utility/enable_if.hpp>

namespace luabind { namespace detail {

namespace mpl = boost::mpl;
namespace fty = boost::function_types;

template <class F, class Enable=luabind_unspecified>
struct signature_aux
{
    typedef F type;
};

// Handle boost::function and std::function:
template <template<class> class F, class S>
struct signature_aux<
    F<S>,
    typename mpl::apply< // Enable only if F<S>::result_type exists.
        mpl::always<void>, typename F<S>::result_type>::type >
{
    typedef S type;
};

template <class F>
struct signature_aux<const F>: signature_aux<F> {};

template <typename F>
struct is_function:
    fty::is_callable_builtin<typename signature_aux<F>::type>
{};

template <class F>
typename boost::enable_if<
    is_function<F>,
    fty::components<typename signature_aux<F>::type> >::type
deduce_signature(F const&, ...)
{
    return fty::components<typename signature_aux<F>::type>();
}

template <class F, class Wrapped>
typename boost::enable_if<
    fty::is_member_function_pointer<typename signature_aux<F>::type>,
    fty::components<
        typename signature_aux<F>::type,
        boost::add_reference<most_derived<mpl::_1, Wrapped> > > >::type
deduce_signature(F, Wrapped*)
{
    return fty::components<
        typename signature_aux<F>::type,
        boost::add_reference<most_derived<mpl::_1, Wrapped> > >();
}

} } // namespace luabind::detail

#  endif // LUABIND_NO_BOOST_FUNCTION_TYPES

# endif // LUABIND_DEDUCE_SIGNATURE_080911_HPP

#else // BOOST_PP_IS_ITERATING

# define N BOOST_PP_ITERATION()
# define NPLUS1 BOOST_PP_INC(N)

template <class R, BOOST_PP_ENUM_PARAMS(N,class A)>
BOOST_PP_CAT(mpl::vector,NPLUS1)<R, BOOST_PP_ENUM_PARAMS(N,A)>
deduce_signature(R(*)(BOOST_PP_ENUM_PARAMS(N,A)), ...)
{
    return BOOST_PP_CAT(mpl::vector,NPLUS1)<R,BOOST_PP_ENUM_PARAMS(N,A)>();
}

# define NPLUS2 BOOST_PP_INC(NPLUS1)

template <class R, class T, BOOST_PP_ENUM_PARAMS(N,class A)>
BOOST_PP_CAT(mpl::vector,NPLUS2)<R, T&, BOOST_PP_ENUM_PARAMS(N,A)>
deduce_signature(R(T::*)(BOOST_PP_ENUM_PARAMS(N,A)))
{
    return BOOST_PP_CAT(mpl::vector,NPLUS2)<R,T&,BOOST_PP_ENUM_PARAMS(N,A)>();
}

template <class R, class T, BOOST_PP_ENUM_PARAMS(N,class A), class Wrapped>
BOOST_PP_CAT(mpl::vector,NPLUS2)<
    R, typename most_derived<T,Wrapped>::type&, BOOST_PP_ENUM_PARAMS(N,A)
>
deduce_signature(R(T::*)(BOOST_PP_ENUM_PARAMS(N,A)), Wrapped*)
{
    return BOOST_PP_CAT(mpl::vector,NPLUS2)<
        R,typename most_derived<T,Wrapped>::type&,BOOST_PP_ENUM_PARAMS(N,A)>();
}

template <class R, class T, BOOST_PP_ENUM_PARAMS(N,class A)>
BOOST_PP_CAT(mpl::vector,NPLUS2)<R, T const&, BOOST_PP_ENUM_PARAMS(N,A)>
deduce_signature(R(T::*)(BOOST_PP_ENUM_PARAMS(N,A)) const)
{
    return BOOST_PP_CAT(mpl::vector,NPLUS2)<R,T const&,BOOST_PP_ENUM_PARAMS(N,A)>();
}

template <class R, class T, BOOST_PP_ENUM_PARAMS(N,class A), class Wrapped>
BOOST_PP_CAT(mpl::vector,NPLUS2)<
    R, typename most_derived<T,Wrapped>::type const&, BOOST_PP_ENUM_PARAMS(N,A)
>
deduce_signature(R(T::*)(BOOST_PP_ENUM_PARAMS(N,A)) const, Wrapped*)
{
    return BOOST_PP_CAT(mpl::vector,NPLUS2)<
        R,typename most_derived<T,Wrapped>::type const&,BOOST_PP_ENUM_PARAMS(N,A)>();
}

# undef NPLUS2
# undef NPLUS1
# undef N

#endif  // BOOST_PP_IS_ITERATING
