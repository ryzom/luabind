#ifndef LUABIND_BOOST_OPTIONAL_HPP_INCLUDED
#define LUABIND_BOOST_OPTIONAL_HPP_INCLUDED

#include <luabind/config.hpp>

#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)

// Boost.Optional 1.34 uses type_with_alignment<> inside a union, which VC6
// cannot parse. A placement-new buffer is enough for luabind's use
// (empty vs a value of a copyable T).
#include <new>
#include <cstddef>

namespace boost {

template <class T>
class optional
{
    bool m_has;
    union {
        char m_buf[sizeof(T)];
        double m_align_d;
        void* m_align_p;
    };
    T* ptr() { return reinterpret_cast<T*>(m_buf); }
    T const* ptr() const { return reinterpret_cast<T const*>(m_buf); }

public:
    optional() : m_has(false) {}
    optional(T const& v) : m_has(true) { new (ptr()) T(v); }
    optional(optional const& o) : m_has(o.m_has)
    {
        if (m_has) new (ptr()) T(*o.ptr());
    }
    ~optional() { if (m_has) ptr()->~T(); }

    optional& operator=(optional const& o)
    {
        if (this != &o)
        {
            if (m_has) ptr()->~T();
            m_has = o.m_has;
            if (m_has) new (ptr()) T(*o.ptr());
        }
        return *this;
    }

    bool operator!() const { return !m_has; }
    T& operator*() { return *ptr(); }
    T const& operator*() const { return *ptr(); }
};

} // namespace boost

#else
# include <boost/optional/optional.hpp>
#endif

#endif
