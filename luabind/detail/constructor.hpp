// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

# ifndef LUABIND_DETAIL_CONSTRUCTOR_081018_HPP
#  define LUABIND_DETAIL_CONSTRUCTOR_081018_HPP

#  include <luabind/detail/inheritance.hpp>
#  include <luabind/get_main_thread.hpp>
#  include <luabind/detail/object.hpp>
#  include <luabind/luabind_memory.hpp>
#  include <luabind/wrapper_base.hpp>

#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/repetition/enum_binary_params.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>

namespace luabind { namespace detail {

inline void inject_backref(lua_State*, void*, void*)
{}

template <class T>
void inject_backref(lua_State* L, T* p, wrap_base*)
{
    weak_ref(get_main_thread(L), L, 1).swap(wrap_access::ref(*p));
}

// Overloads of operator() instead of construct_aux<N, ...> partial
// specializations — VC6 has no partial spec, and explicit specialization
// of a nested-template wrapper is C2011 on cl 12.00.
template <class T, class Pointer, class Signature>
struct construct
{
    typedef pointer_holder<Pointer, T> holder_type;

    void operator()(argument const& self_) const
    {
        object_rep* self = touserdata<object_rep>(self_);
        luabind::unique_ptr<T> instance(new T);
        inject_backref(self_.interpreter(), instance.get(), instance.get());

        void* naked_ptr = instance.get();
        Pointer ptr(instance.release());

        void* storage = self->allocate(sizeof(holder_type));

        self->set_instance(new (storage) holder_type(
            luabind::move(ptr), registered_class<T>::id, naked_ptr));
    }

#  define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (1, LUABIND_MAX_ARITY, <luabind/detail/constructor.hpp>))
#  include BOOST_PP_ITERATE()
};

}} // namespace luabind::detail

# endif // LUABIND_DETAIL_CONSTRUCTOR_081018_HPP

#else // BOOST_PP_IS_ITERATING

# define N BOOST_PP_ITERATION()

    template <BOOST_PP_ENUM_PARAMS(N, class A)>
    void operator()(argument const& self_, BOOST_PP_ENUM_BINARY_PARAMS(N, A, a)) const
    {
        object_rep* self = touserdata<object_rep>(self_);

        luabind::unique_ptr<T> instance(new T(BOOST_PP_ENUM_PARAMS(N, a)));
        inject_backref(self_.interpreter(), instance.get(), instance.get());

        void* naked_ptr = instance.get();
        Pointer ptr(instance.release());

        void* storage = self->allocate(sizeof(holder_type));

        self->set_instance(new (storage) holder_type(
            luabind::move(ptr), registered_class<T>::id, naked_ptr));
    }

# undef N

#endif
