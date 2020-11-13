#ifndef EXTENSIBLE_REALOC_STRATEGY_H_INCLUDED
#define EXTENSIBLE_REALOC_STRATEGY_H_INCLUDED

//extensible strategy with reallocation for allocating only one element a time

#include <vector>

template<class T, std::size_t SZ>
class ext_realloc_strategy
{
public:
    ext_realloc_strategy() noexcept;
    ext_realloc_strategy(const ext_realloc_strategy&) = delete;
    ~ext_realloc_strategy();

    T* allocate(std::size_t);
    void deallocate(T*, std::size_t);

private:
    void* add_block_();

private:
    struct free_block{
        free_block* next;
    };
    free_block* free_ptr;
    std::vector<void*> keeper_;
    std::size_t type_size_;
};


template<typename T, std::size_t SZ>
inline ext_realloc_strategy<T, SZ>::ext_realloc_strategy() noexcept : type_size_(0) {}


template<typename T, std::size_t SZ>
inline ext_realloc_strategy<T, SZ>::~ext_realloc_strategy() {
    for (auto it : keeper_)
        ::operator delete(it);
}

template<typename T, std::size_t SZ>
inline void* ext_realloc_strategy<T, SZ>::add_block_() {

    if(!type_size_)
        type_size_ = sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*);

    void* chunk_ = static_cast<T*>(::operator new (SZ*type_size_));

    if(!chunk_)
        throw std::bad_alloc();

    std::size_t count = SZ;
    char* p;
    for(p = reinterpret_cast<char*>(chunk_); count > 1; --count, p += type_size_)
        *reinterpret_cast<char**>(p) = p + type_size_;
    *reinterpret_cast<char**>(p) = nullptr;

    keeper_.push_back(chunk_);
    return chunk_;
}

template<typename T, std::size_t SZ>
inline T* ext_realloc_strategy<T, SZ>::allocate(std::size_t n) {

    if(n == 0)
        return nullptr;

    if(n > 1)
        throw std::bad_alloc();

    if(keeper_.empty()) {            // memory is reserved at the first allocation
        keeper_.reserve(10);        // reducing memory reallocation when expanding storage
        free_ptr = static_cast<free_block*>(add_block_());
    }

    if(!free_ptr)
        free_ptr = static_cast<free_block*>(add_block_());

    auto p = free_ptr;
    free_ptr = free_ptr->next;
    return reinterpret_cast<T*>(p);
}

template<typename T, std::size_t SZ>
inline void ext_realloc_strategy<T, SZ>::deallocate(T* ptr_, std::size_t n){

    if(ptr_ == nullptr || n == 0)
        return;

    auto p = reinterpret_cast<free_block*>(ptr_);
    p->next = free_ptr;
    free_ptr = p;
}

#endif // EXTENSIBLE_REALOC_STRATEGY_H_INCLUDED
