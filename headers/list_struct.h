#ifndef LIST_STRUCT_H_INCLUDED
#define LIST_STRUCT_H_INCLUDED

#define _PRETTY_ __PRETTY_FUNCTION__
#if (defined NDEBUG)
#define DEBUG_MODE(x)
#else
#define DEBUG_MODE(x) (x)
#endif

template <typename T>
struct bunch {
	T f;
	T s;
	bunch() : f(T()), s(T()) {}
	bunch(T f_, T s_) : f(f_), s(s_) {}
	bunch(const bunch&) = default;
	bunch& operator=(const bunch&) = default;
	bunch(bunch&&) = default;
};


template <typename T>
struct node_ {
	node_<T>* next;
	T data;
};


template <typename T, typename Allocator = std::allocator<T>>
class LinkedList;


template <typename T>
class LListIterator : public std::iterator<std::forward_iterator_tag, T> {
public:

    LListIterator() = default;
    LListIterator(node_<T>* p) : pc_(p) {}

    bool operator!=(const LListIterator& it) const {
        return pc_ != it.pc_;
    }

    bool operator==(const LListIterator& it) const {
        return pc_ == it.pc_;
    }

    LListIterator<T>& operator++() {
        pc_ = pc_->next;
        return *this;
    }

     LListIterator<T> operator++(int) {
        LListIterator<T> tmp = *this;
        pc_ = pc_->next;
        return tmp;
      }

    node_<T>* pointer_to_node() const {return pc_;}

    typename LListIterator::reference operator*() const {
        return pc_->data;
    }

    typename LListIterator::pointer operator->() const {
        return pc_->data;
    }

    template <typename U, typename Allocator>
    friend class LinkedList;

private:
    node_<T>* pc_ = nullptr;
};


template <typename T, typename Allocator>
class LinkedList {

private:
    node_<T>* pHead = nullptr;
    node_<T>* pCur  = nullptr;
    node_<T> nBef;
    /*
    using Alloc_type_T = typename std::allocator_traits<Allocator>::allocator_type::template rebind<T>::other;
    using Alloc_traits_T = std::allocator_traits<Alloc_type_T>;
    using Alloc_type_node = typename Alloc_traits_T::allocator_type::template rebind<node_<T>>::other;
    using Alloc_traits_node = std::allocator_traits<Alloc_type_node>;
    Alloc_traits_node NodeAllocator;
    */
    typename Allocator::template rebind<node_<T>>::other NodeAllocator;
public:
    using iterator = LListIterator<T>;
    using size_type = size_t;

    LinkedList() = default;

    LinkedList(const LinkedList&);

    LinkedList(LinkedList&&) noexcept;

    template <typename Alloc_>
    LinkedList(const LinkedList<T,Alloc_>&);

    template <typename Alloc_>
    LinkedList(LinkedList<T,Alloc_>&&);

	~LinkedList();

	void Add(const T& _data);

    template <typename ...Args>
    void Emplace(Args&& ...args);

    T front();
    bool empty();
    void clear();
	bool GetNext(T&);


	LinkedList::iterator erase_after(iterator);
	LinkedList::iterator erase_after(const iterator, const iterator);

    LinkedList::iterator before_begin();
    LinkedList::iterator begin() {return iterator(pHead);}
    LinkedList::iterator end()   {return iterator();}

    node_<T>* GetHead() const {return pHead;}
#ifndef NDEBUG
    void* Address_of_Head() const {return pHead;}
    void* Address_of_Head_Data() const {return pHead ? &(pHead->data) : nullptr;}
#endif
    void HeadToNull() {pHead = nullptr;}
};


template <typename T, typename Allocator>
inline LinkedList<T,Allocator>::LinkedList(const LinkedList<T,Allocator>& ob){
    DEBUG_MODE(std::cout << _PRETTY_ << std::endl);

    node_<T>* pCurOb = ob.pHead;
    while(pCurOb) {
        Add(pCurOb->data);
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
template <typename Alloc_>
inline LinkedList<T, Allocator>::LinkedList(const LinkedList<T,Alloc_>& ob) {
    DEBUG_MODE(std::cout << _PRETTY_ << std::endl);

    node_<T>* pCurOb = ob.GetHead();
    while(pCurOb) {
        Add(pCurOb->data);
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
inline LinkedList<T, Allocator>::LinkedList(LinkedList<T,Allocator>&& ob) noexcept
        : pHead(ob.GetHead()) {
    DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
    ob.HeadToNull();
}


template <typename T, typename Allocator>
template <typename Alloc_>
inline LinkedList<T, Allocator>::LinkedList(LinkedList<T,Alloc_>&& ob) {
    DEBUG_MODE(std::cout << _PRETTY_ << std::endl);
    node_<T>* pCurOb = ob.GetHead();
    while(pCurOb) {
        Add(std::move(pCurOb->data));
        pCurOb = pCurOb->next;
    }
    ob.HeadToNull();
}


template <typename T, typename Allocator>
inline LinkedList<T, Allocator>::~LinkedList() {

	clear();
}


template <typename T, typename Allocator>
inline void LinkedList<T, Allocator>::clear() {

	node_<T>* temp;
	pCur = pHead;
	while (pCur){
		temp = pCur;
		pCur = pCur->next;

        NodeAllocator.destroy(&temp->data);
        NodeAllocator.deallocate(temp, 1u);
	}
	HeadToNull();
}


template <typename T, typename Allocator>
inline void LinkedList<T, Allocator>::Add(const T& _data) {

    node_<T>* temp;
    node_<T>* newNode = NodeAllocator.allocate(1u);

    NodeAllocator.construct(&newNode->data, _data);

	newNode->next = nullptr;

	if (!pHead)
		pHead = pCur = newNode;
	else {
		pCur = pHead;
		while(pCur) {
			temp = pCur;
			pCur = pCur->next;
		}
		temp->next = newNode;
	}
}


template <typename T, typename Allocator>
template <typename ...Args>
inline void LinkedList<T, Allocator>::Emplace(Args&& ...args) {

    node_<T>* temp;
    node_<T>* newNode = NodeAllocator.allocate(1u);
    NodeAllocator.construct(&newNode->data, std::forward<Args>(args)...);
    newNode->next = nullptr;

    if (!pHead)
        pHead = pCur = newNode;
    else {
        pCur = pHead;
        while(pCur) {
          temp = pCur;
          pCur = pCur->next;
        }
        temp->next = newNode;
    }
}


template <typename T, typename Allocator>
inline bool LinkedList<T, Allocator>::GetNext(T& _data) {

    node_<T>* temp;
	if (pCur) {
		temp = pCur;
		pCur = pCur->next;
		_data = temp->data;
		return true;
	}
	else
		return false;
}


template <typename T, typename Allocator>
inline T LinkedList<T, Allocator>::front() {
    return *begin();
}


template <typename T, typename Allocator>
inline bool LinkedList<T, Allocator>::empty() {
    return !pHead;
}


template <typename T, typename Allocator>
inline typename LinkedList<T, Allocator>::iterator
  LinkedList<T, Allocator>::before_begin() {

    nBef.next = pHead;
    return iterator(&nBef);
}


template <typename T, typename Allocator>
inline typename LinkedList<T, Allocator>::iterator
  LinkedList<T, Allocator>::erase_after(iterator it_) {

    if(it_ != end()){
        node_<T>* temp = it_.pointer_to_node()->next;
        if(temp){
            it_.pointer_to_node()->next = temp->next;
            NodeAllocator.destroy(&temp->data);
            NodeAllocator.deallocate(temp, 1u);
        }
    }

    return ++it_;
}

#endif // LIST_STRUCT_H_INCLUDED
