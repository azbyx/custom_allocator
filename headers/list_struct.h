#ifndef LIST_STRUCT_H_INCLUDED
#define LIST_STRUCT_H_INCLUDED

#define _PRETTY_ __PRETTY_FUNCTION__

template <typename T>
struct bunch {
	T f;
	T s;
};


template <typename T>
struct node_ {
	node_<T>* next;
	T data;
};


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

    typename LListIterator::reference operator*() const {
        return pc_->data;
    }

    typename LListIterator::pointer operator->() const {
        return pc_->data;
    }

private:
    node_<T>* pc_ = nullptr;
};


template <typename T, typename Allocator = std::allocator<T>>
class LinkedList {

private:
    node_<T>* pHead = nullptr;
    node_<T>* pCur  = nullptr;
    node_<T> nBef;

    typename Allocator::template rebind<node_<T>>::other mem_;

public:
    using iterator = LListIterator<T>;
    using const_iterator = LListIterator<const T>;

    LinkedList() = default;

    LinkedList(const LinkedList&);

    LinkedList(LinkedList&&) noexcept;

    template <typename Alloc_>
    LinkedList(const LinkedList<T,Alloc_>&);

    template <typename Alloc_>
    LinkedList(LinkedList<T,Alloc_>&&);

	~LinkedList();

	void Add(const T& _data);
	void Add(T&& _data);

    template <typename ...Args>
    void Emplace(Args&& ...args);

    T front();
    bool empty();
    void clear();
	bool GetNext(T&);

    LinkedList::iterator before_begin();
    LinkedList::iterator begin() {return iterator(pHead);}
    LinkedList::iterator end()   {return iterator();}
    LinkedList::const_iterator begin() const {return const_iterator(pHead);}
    LinkedList::const_iterator end() const  {return const_iterator();}

    node_<T>* GetHead() const {return pHead;}

    void* Address_of_Head() const {return pHead;}
    void* Address_of_Head_Data() const {return pHead ? &(pHead->data) : nullptr;}

    void HeadToNull() {pHead = nullptr;}
};


template <typename T, typename Allocator>
inline LinkedList<T,Allocator>::LinkedList(const LinkedList<T,Allocator>& ob){
    std::cout << _PRETTY_ << std::endl;

    node_<T>* pCurOb = ob.pHead;
    while(pCurOb) {
        Add(pCurOb->data);
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
template <typename Alloc_>
inline LinkedList<T, Allocator>::LinkedList(const LinkedList<T,Alloc_>& ob) {
    std::cout << _PRETTY_ << std::endl;

    node_<T>* pCurOb = ob.GetHead();
    while(pCurOb) {
        Add(pCurOb->data);
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
inline LinkedList<T, Allocator>::LinkedList(LinkedList<T,Allocator>&& ob) noexcept
        : pHead(ob.GetHead()) {
    std::cout << _PRETTY_ << std::endl;
    ob.HeadToNull();
}


template <typename T, typename Allocator>
template <typename Alloc_>
inline LinkedList<T, Allocator>::LinkedList(LinkedList<T,Alloc_>&& ob) {
    std::cout << _PRETTY_ << std::endl;
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

        mem_.destroy(&temp->data);
        mem_.deallocate(temp, 1);
	}
	HeadToNull();
}


template <typename T, typename Allocator>
inline void LinkedList<T, Allocator>::Add(const T& _data) {

    node_<T>* temp;
    node_<T>* newNode = mem_.allocate(1u);

    mem_.construct(&newNode->data, _data);

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
inline void LinkedList<T, Allocator>::Add(T&& _data) {
    std::cout << _PRETTY_ << std::endl;
    node_<T>* temp;
    node_<T>* newNode = mem_.allocate(1u);
    mem_.construct(&newNode->data, std::move(_data));

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
    node_<T>* newNode = mem_.allocate(1u);
    mem_.construct(&newNode->data, std::forward<Args>(args)...);
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

#endif // LIST_STRUCT_H_INCLUDED
