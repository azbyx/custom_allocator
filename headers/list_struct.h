#ifndef LIST_STRUCT_H_INCLUDED
#define LIST_STRUCT_H_INCLUDED

template <typename T>

struct node_ {
	T data;
	node_<T>* next;
};



template <typename T>
class MyIterator {
private:
    node_<T>* pCur = nullptr;

public:
    MyIterator() = default;
    MyIterator(node_<T>* p) : pCur(p) {}

    bool operator!=(const MyIterator& it) const {
        return pCur != it.pCur;
    }

    const MyIterator& operator++() {
        pCur = pCur->next;
        return *this;
    }

    const T& operator*() const {
        return pCur->data;
    }

    T& operator*() {
        return pCur->data;
    }
};


template <typename T, typename Allocator = std::allocator<T>>
class LinkedList_like {

private:
    node_<T>* pHead = nullptr;
    node_<T>* pCur  = nullptr;

    // It's still unclear a bit
    using ActualAlloc = Allocator::template rebind<node_<T>>::other;

public:
    //template <typename Alloc_>
    //friend LinkedList_like<T,Alloc_>; // Why doesn't it work?

    LinkedList_like() = default;

    LinkedList_like(const LinkedList_like& ob);

    LinkedList_like(LinkedList_like&& ob) noexcept;

    template <typename Alloc_>
    LinkedList_like(const LinkedList_like<T,Alloc_>& ob);

    template <typename Alloc_>
    LinkedList_like(LinkedList_like<T,Alloc_>&& ob);

	~LinkedList_like();

	void Add(T _data);

    template <typename ...Args>
    void Emplace(Args ...args);

	void PrepareToRead();
	bool GetNext(T& _data);

    MyIterator<T> begin() {return MyIterator<T>(pHead);}

    MyIterator<T> end()   {return MyIterator<T>();}

    node_<T>* GetHead() const {return pHead;}

    void HeadToNull() {pHead = nullptr;}

};


template <typename T, typename Allocator>
LinkedList_like<T,Allocator>::LinkedList_like(const LinkedList_like<T,Allocator>& ob) {

    if (!ob.pHead)
        return;

    pHead = ActualAlloc.allocate(1);
    ActualAlloc.construct(&pHead->data, ob.pHead->data);
    pHead->next = nullptr;
    pCur = pHead;

    node_<T>* newNode;
    node_<T>* pCurOb = ob.pHead->next;

    while (pCurOb) {
        newNode = ActualAlloc.allocate(1);
        ActualAlloc.construct(&newNode->data, pCurOb->data);
        newNode->next = nullptr;
        pCur->next = newNode;
        pCur = pCur->next;
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
template <typename Alloc_>
LinkedList_like<T, Allocator>::LinkedList_like(const LinkedList_like<T,Alloc_>& ob) {

    if (!ob.GetHead())
        return;

    pHead = ActualAlloc.allocate(1);
    ActualAlloc.construct(&pHead->data, ob.GetHead()->data);
    pHead->next = nullptr;
    pCur = pHead;

    node_<T>* newNode;
    node_<T>* pCurOb = ob.GetHead()->next;

    while (pCurOb) {
        newNode = ActualAlloc.allocate(1);
        ActualAlloc.construct(&newNode->data, pCurOb->data);
        newNode->next = nullptr;

        pCur->next = newNode;
        pCur = pCur->next;
        pCurOb = pCurOb->next;
    }
}


template <typename T, typename Allocator>
template <typename Alloc_>
LinkedList_like<T, Allocator>::LinkedList_like(LinkedList_like<T,Alloc_>&& ob) {

    if (!ob.GetHead())
        return;

    pHead = ActualAlloc.allocate(1);
    ActualAlloc.construct(&pHead->data, std::move(ob.GetHead()->data));
    pHead->next = nullptr;
    pCur = pHead;

    node_<T>* newNode;
    node_<T>* pCurOb = ob.GetHead()->next;

    while (pCurOb) {
        newNode = ActualAlloc.allocate(1);
        ActualAlloc.construct(&newNode->data, std::move(pCurOb->data));
        newNode->next = nullptr;
        pCur->next = newNode;
        pCur = pCur->next;
        pCurOb = pCurOb->next;
    }

    ob.HeadToNull();
}


template <typename T, typename Allocator>
LinkedList_like<T, Allocator>::LinkedList_like(LinkedList_like<T,Allocator>&& ob) noexcept
        : pHead(ob.pHead), ActualAlloc(std::move(ob.ActualAlloc)) {

    ob.pHead = nullptr;
}


template <typename T, typename Allocator>
LinkedList_like<T, Allocator>::~LinkedList_like() {

	node_<T>* temp;
	pCur = pHead;
	while (pCur){
		temp = pCur;
		pCur = pCur->next;

        ActualAlloc.destroy(&temp->data); // temp or data? I think 'data' cos we constructed 'data' before
        ActualAlloc.deallocate(temp, 1);  // temp or data? I think 'temp' cos we allocated node_<T>
	}
}



template <typename T, typename Allocator>
void LinkedList_like<T, Allocator>::Add(T _data) {

    node_<T>* temp;
    node_<T>* newNode = ActualAlloc.allocate(1);

    ActualAlloc.construct(&newNode->data, std::forward<hard>(_data));

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
void LinkedList_like<T, Allocator>::Emplace(Args&& ...args) {

    node_<T>* temp;
    node_<T>* newNode = ActualAlloc.allocate(1);
    ActualAlloc.construct(&newNode->data, std::forward<Args>(args)...);
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
void LinkedList_like<T, Allocator>::PrepareToRead() {

    pCur = pHead;
}



template <typename T, typename Allocator>
bool LinkedList_like<T, Allocator>::GetNext(T& _data) {

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
