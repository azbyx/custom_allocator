#ifndef EXTENSIBLE_SIMPLE_STRATEGY_H_INCLUDED
#define EXTENSIBLE_SIMPLE_STRATEGY_H_INCLUDED

#include <vector>

template<class T, std::size_t SZ>
class ext_simple_strategy
{
public:
    ext_simple_strategy() noexcept;
    ext_simple_strategy(const ext_simple_strategy&) = delete;
    ~ext_simple_strategy();

    T* allocate(std::size_t);
    void deallocate(T*, std::size_t);

private:
    void add_block_();

private:
    std::vector<T*> keeper_;
    std::size_t used_;
};


template<typename T, std::size_t SZ>
inline ext_simple_strategy<T, SZ>::ext_simple_strategy() noexcept : used_(0) {}


template<typename T, std::size_t SZ>
inline ext_simple_strategy<T, SZ>::~ext_simple_strategy() {
    for (auto it : keeper_)
        ::operator delete(it);
}

template<typename T, std::size_t SZ>
inline void ext_simple_strategy<T, SZ>::add_block_() {
    T* chunk_ = static_cast<T*>(::operator new (SZ*sizeof(T)));
    if(!chunk_)
        throw std::bad_alloc();
    keeper_.push_back(chunk_);
}

template<typename T, std::size_t SZ>
inline T* ext_simple_strategy<T, SZ>::allocate(std::size_t n) {
    if(n == 0)
        return nullptr;

    if(n > SZ)
        throw std::bad_alloc();

    if(keeper_.empty()) {            // memory is reserved at the first allocation
        keeper_.reserve(10);        // reducing memory reallocation when expanding storage
        add_block_();
    }

    std::size_t cur_index = used_/SZ;
    std::size_t cur_pointer = used_%SZ;
    std::size_t cur_remainder = (SZ - cur_pointer)%SZ;

    if(cur_remainder < n) {
        used_ += (cur_remainder + n); // if n > 1, memory overhead is possible here
        add_block_();                 // storage expansion
        T* res = keeper_[cur_index + 1];
        return res;
    }
    else {
        T* res = keeper_[cur_index] + cur_pointer;
        used_ += n;
        return res;
    }
}

template<typename T, std::size_t SZ>
inline void ext_simple_strategy<T, SZ>::deallocate(T*, std::size_t){} // memory after deallocation is not reused

#endif // EXTENSIBLE_SIMPLE_STRATEGY_H_INCLUDED
