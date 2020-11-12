#ifndef LIST_STRUCT_H_INCLUDED
#define LIST_STRUCT_H_INCLUDED

#define _PRETTY_ __PRETTY_FUNCTION__

template <typename T>
struct node_ {
	T data;
	node_<T>* next;
};


template <typename T>
class LListIterator : public std::iterator<std::input_iterator_tag, T> {
public:

    LListIterator() = default;
    LListIterator(node_<T>* p) : pCur(p) {}

    bool operator!=(const LListIterator& it) const {
        return pCur != it.pCur;
    }

    bool operator==(const LListIterator& it) const {
        return pCur == it.pCur;
    }

    LListIterator<T>& operator++() {
        pCur = pCur->next;
        return *this;
    }

     LListIterator<T> operator++(int) {
        LListIterator<T> tmp = *this;
        pCur = pCur->next;
        return tmp;
      }

    typename LListIterator::reference operator*() const {
        return pCur->data;
    }

    typename LListIterator::pointer operator->() const {
        return pCur->data;
    }

private:
    node_<T>* pCur = nullptr;
};


template <typename T, typename Allocator = std::allocator<T>>
class LinkedList {

private:
    node_<T>* pHead = nullptr;
    node_<T>* pCur  = nullptr;

    using Node_Alloc_traits = typename Allocator::template rebind<node_<T>>::other;
    Node_Alloc_traits mem;

public:

    using iterator = LListIterator<T>;
    using const_iterator = LListIterator<const T>;
    //template <typename Alloc_>
    //friend LinkedList<T,Alloc_>; // Why doesn't it work?

    LinkedList() = default;

    LinkedList(const LinkedList&);

    LinkedList(LinkedList&&) noexcept;

    template <typename Alloc_>
    LinkedList(const LinkedList<T,Alloc_>&);

    template <typename Alloc_>
    LinkedList(LinkedList<T,Alloc_>&&);

	~LinkedList();

	void Add(T _data);

    template <typename ...Args>
    void Emplace(Args&& ...args);

	bool GetNext(T&);

    LinkedList::iterator begin() {return iterator(pHead);}
    LinkedList::iterator end()   {return iterator();}
    LinkedList::const_iterator begin() const {return const_iterator(pHead);}
    LinkedList::const_iterator end() const  {return const_iterator();}

    node_<T>* GetHead() const {return pHead;}

    void HeadToNull() {pHead = nullptr;}

};


template <typename T, typename Allocator>
LinkedList<T,Allocator>::LinkedList(const LinkedList<T,Allocator>& ob){
    std::cout << _PRETTY_ << std::endl;

    node_<T>* pCurOb = ob.GetHead();
    while(pCurOb) {
        Add(pCurOb->data);
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
template <typename Alloc_>
LinkedList<T, Allocator>::LinkedList(const LinkedList<T,Alloc_>& ob) {
    std::cout << _PRETTY_ << std::endl;

    node_<T>* pCurOb = ob.GetHead();
    while(pCurOb) {
        Add(pCurOb->data);
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(LinkedList<T,Allocator>&& ob) noexcept
        : pHead(ob.GetHead()) {
    std::cout << _PRETTY_ << std::endl;
    ob.HeadToNull();
}

template <typename T, typename Allocator>
template <typename Alloc_>
LinkedList<T, Allocator>::LinkedList(LinkedList<T,Alloc_>&& ob) {
    std::cout << _PRETTY_ << std::endl;
    if (!ob.GetHead())
        return;

    pHead = mem.allocate(1u);
    mem.construct(&pHead->data, std::move(ob.GetHead()->data));
    pHead->next = nullptr;
    pCur = pHead;

    node_<T>* newNode;
    node_<T>* pCurOb = ob.GetHead()->next;

    while (pCurOb) {
        newNode = mem.allocate(1u);
        mem.construct(&newNode->data, std::move(pCurOb->data));
        newNode->next = nullptr;
        pCur->next = newNode;
        pCur = pCur->next;
        pCurOb = pCurOb->next;
    }
    ob.HeadToNull();
}



template <typename T, typename Allocator>
LinkedList<T, Allocator>::~LinkedList() {

	node_<T>* temp;
	pCur = pHead;
	while (pCur){
		temp = pCur;
		pCur = pCur->next;

        mem.destroy(&temp->data); // temp or data? I think 'data' cos we constructed 'data' before
        mem.deallocate(temp, 1);  // temp or data? I think 'temp' cos we allocated node_<T>
	}
}


template <typename T, typename Allocator>
void LinkedList<T, Allocator>::Add(T _data) {

    node_<T>* temp;
    node_<T>* newNode = mem.allocate(1u);

    mem.construct(&newNode->data, _data);

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
void LinkedList<T, Allocator>::Emplace(Args&& ...args) {

    node_<T>* temp;
    node_<T>* newNode = mem.allocate(1u);
    mem.construct(&newNode->data, std::forward<Args>(args)...);
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
bool LinkedList<T, Allocator>::GetNext(T& _data) {

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

#endif // LIST_STRUCT_H_INCLUDED
