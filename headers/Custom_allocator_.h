#ifndef CUSTOM_ALLOCATOR_H_INCLUDED
#define CUSTOM_ALLOCATOR_H_INCLUDED

#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <utility>
#include <memory>

#define _PRETTY_ __PRETTY_FUNCTION__

#if (defined NDEBUG)
#define DEBUG_MODE(x)
#else
#define DEBUG_MODE(x) (x)
#endif


template <typename T, size_t BLOCKS, template<class, size_t> class MEMCONTROL>
class Custom_allocator
{
private:
    T *pMemory = nullptr;
    unsigned takens = 0;

public:
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = Custom_allocator<U,BLOCKS, MEMCONTROL>;
    };

    //Custom_allocator(const Custom_allocator &) = default;
    Custom_allocator(const Custom_allocator &) = delete;

    Custom_allocator(Custom_allocator &&ob) noexcept
    {
        DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
        pMemory = ob.pMemory;
        takens = ob.takens;
        ob.pMemory = nullptr;
        ob.takens = 0;
    }

    /*template<typename U>
    Custom_allocator(const Custom_allocator<U,BLOCKS> &)  // ???  Added for C++Builder
    {
        std::cout << _PRETTY_ << " (Added for C++Builder)" << std::endl; // never called
    }*/

    T* allocate( /*[[maybe_unused]]*/ std::size_t n )
    {
        DEBUG_MODE(std::cout << _PRETTY_ << "[n = " << n << "]" << std::endl);

        //if (n != 1)              // Not really necessary for plain allocator
        //    throw std::bad_alloc();

        T *res = pMemory+takens;
        takens += n;
        if (takens <= BLOCKS)
            return res;
        else
        {
            throw std::bad_alloc();
            //return nullptr;
        }
    }

    void deallocate( [[maybe_unused]] T* p, [[maybe_unused]] std::size_t n ) const
    {
        DEBUG_MODE(std::cout << _PRETTY_ << "[n = " << n << "]" << std::endl);
        //std::free(p); // not necessary cos we dealloacate whole memory at once
    }

    template<typename U, typename ...Args>
    void construct(U* p, Args&& ...args) const
    {
        DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p) const
    {
        DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
        p->~U();
    }

    Custom_allocator()
    {
        DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
        pMemory = (T*)std::malloc(BLOCKS * sizeof(T));
    }

    ~Custom_allocator()
    {
        DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
        if (pMemory)
            std::free(pMemory);
    }
};

#endif // CUSTOM_ALLOCATOR_H_INCLUDED
