#ifndef SYMPLE_STRATEGY_H
#define SYMPLE_STRATEGY_H

template<class T, std::size_t SZ>
class simple_strategy
{
public:
    simple_strategy() noexcept;
    simple_strategy(const simple_strategy&) = delete;
    ~simple_strategy();

    T* allocate(std::size_t);
    void deallocate(T*, std::size_t);

private:
    T* chunk_;
    std::size_t used_;
};


template<typename T, std::size_t SZ>
inline simple_strategy<T, SZ>::simple_strategy() noexcept :
    chunk_(nullptr), used_(0) {}


template<typename T, std::size_t SZ>
inline simple_strategy<T, SZ>::~simple_strategy() {
    ::operator delete(chunk_);
}

template<typename T, std::size_t SZ>
inline T* simple_strategy<T, SZ>::allocate(std::size_t n) {
    if(n == 0)
        return nullptr;

    if(!chunk_){
        chunk_ = static_cast<T*>(::operator new (SZ*sizeof(T))); // memory is reserved at the first allocation
        if(!chunk_)
            throw std::bad_alloc();
    }

    T* res = chunk_ + used_;
    used_ += n;
    if (used_ > SZ)                     // the total number of allocated objects is limited by SZ
        throw std::bad_alloc();
    else
        return res;
}

template<typename T, std::size_t SZ>
inline void simple_strategy<T, SZ>::deallocate(T*, std::size_t){} // memory after deallocation is not reused


#endif // SYMPLE_STRATEGY_H
