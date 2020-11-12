#ifndef CUSTOM_ALLOCATOR_H_INCLUDED
#define CUSTOM_ALLOCATOR_H_INCLUDED

template <typename T, std::size_t BLOCKS, template<typename, std::size_t> class Strategy>
class Custom_allocator
{
private:
    Strategy<T, BLOCKS> memory;

public:
    using value_type = T;

    template<typename U>
    struct rebind {
        using other = Custom_allocator<U, BLOCKS, Strategy>;
    };

    Custom_allocator() = default;
    ~Custom_allocator() = default;
    Custom_allocator(const Custom_allocator &) = delete;

    /*Custom_allocator(Custom_allocator &&ob) noexcept
    {
        DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
        pMemory = ob.pMemory;
        used = ob.used;
        ob.pMemory = nullptr;
        ob.used = 0;
    }*/

    T* allocate(size_t n) {
        return memory.alloc_(n);
    }

    void deallocate(T* p, std::size_t n ) {
        memory.dealloc_(p, n);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args&& ...args) const {
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U *p) const {
        p->~U();
    }

};

template<typename T, std::size_t BLOCKS, template<typename, std::size_t> class Strategy>
inline bool operator==(const Custom_allocator<T, BLOCKS, Strategy>&, const Custom_allocator<T, BLOCKS, Strategy>&) {
    return false;
}

template<typename T, std::size_t BLOCKS, template<typename, std::size_t> class Strategy>
inline bool operator!=(const Custom_allocator<T, BLOCKS, Strategy>&, const Custom_allocator<T, BLOCKS, Strategy>&) {
    return false;
}

#endif // CUSTOM_ALLOCATOR_H_INCLUDED
