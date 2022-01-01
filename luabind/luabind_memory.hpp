#ifndef LUABIND_MEMORY_HPP_INCLUDED
#define LUABIND_MEMORY_HPP_INCLUDED

#include <luabind/build_information.hpp>
#include <memory>

namespace luabind {

#ifndef LUABIND_NO_STD_UNIQUE_PTR

  using std::unique_ptr;
  using std::move;

#else

  #if __cplusplus < 201103L

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
