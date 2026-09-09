#ifndef LUABIND_MEMORY_HPP_INCLUDED
#define LUABIND_MEMORY_HPP_INCLUDED

#include <luabind/build_information.hpp>
#include <boost/config.hpp>
#include <memory>

namespace luabind {

#ifndef LUABIND_NO_STD_UNIQUE_PTR

  using std::unique_ptr;
  using std::move;

#else

  #if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)

    // Do not `#define unique_ptr auto_ptr`: `luabind::unique_ptr<T>` would
    // expand to `luabind::auto_ptr<T>`, which is not a template-id on VC6.
    template<class T>
    class unique_ptr
    {
        std::auto_ptr<T> m_p;
    public:
        unique_ptr() {}
        explicit unique_ptr(T* p) : m_p(p) {}
        unique_ptr(unique_ptr& o) : m_p(o.m_p) {}
        unique_ptr& operator=(unique_ptr& o)
        {
            m_p = o.m_p;
            return *this;
        }
        T* get() const { return m_p.get(); }
        T* release() { return m_p.release(); }
        void reset(T* p = 0) { m_p.reset(p); }
        T& operator*() const { return *m_p; }
        T* operator->() const { return m_p.get(); }
    };

  #elif __cplusplus < 201103L

    using std::auto_ptr;
    #define unique_ptr auto_ptr

  #else

    template<typename T>
    using unique_ptr = std::auto_ptr<T>;

  #endif

  template<typename T>
  T move(T ptr)
  {
      return ptr;
  }

#endif // LUABIND_NO_STD_UNIQUE_PTR

} // namespace luabind

#endif // LUABIND_MEMORY_HPP_INCLUDED
