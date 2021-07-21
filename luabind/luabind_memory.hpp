#ifndef LUABIND_MEMORY_HPP_INCLUDED
#define LUABIND_MEMORY_HPP_INCLUDED

# include <memory>

namespace luabind{

#if __cplusplus >= 201103L
    using std::unique_ptr;
    using std::move;
#else
    using unique_ptr = std::auto_ptr;
    template<typename T>
    T move(T ptr){
        return ptr;
    }
#endif
}

#endif