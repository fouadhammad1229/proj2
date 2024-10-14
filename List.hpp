#ifndef C_LIST_HPP
#define C_LIST_HPP

using namespace std;
// const_iterator and const T operators
//
template<typename T>
struct Node {
    T data;  // The actual data
    Node* prev;  // Pointer to the previous node
    Node* next;  // Pointer to the next node

    Node(const T& d = T{}, Node* p = nullptr, Node* n = nullptr)
        : data{d}, prev{p}, next{n} {}
};


template<typename T>
List<T>::const_iterator::const_iterator() : current{nullptr} {}

template<typename T>
const T &List<T>::const_iterator::operator*() const {
	return retrieve();
}

template<typename T>
const T *List<T>::const_iterator::operator->() const {
	return &retrieve();
}

// Increment and Decrement Operators

template<typename T>
typename List<T>::const_iterator &List<T>::const_iterator::operator++() {
	current = current->next;
	return *this;
}

template<typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator++(int) {
	const_iterator old = *this;
	++(*this);
	return old;
}

template<typename T>
typename List<T>::const_iterator &List<T>::const_iterator::operator--() {
	current = current->prev;
	return *this;
}

template<typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator--(int) {
	const_iterator old = *this;
	--(*this);
	return old;
}

// Comparison Operators
template<typename T>
bool List<T>::const_iterator::operator==(const const_iterator &rhs) const {
	return current == rhs.current;
}

template<typename T>
bool List<T>::const_iterator::operator!=(const const_iterator &rhs) const {
	return current != rhs.current;
}

template<typename T>
List<T>::const_iterator::const_iterator(Node *p) : current{p} {} // Node *P

// Iterator Class Member Function
template<typename T>
List<T>::iterator::iterator() : const_iterator{} {}

template<typename T>
List<T>::iterator::iterator(Node *p) : const_iterator{p} {}


template<typename T>
T& List<T>::const_iterator::retrieve() const {
    return current->data;  // Return a non-const reference to the data in the node
}

template<typename T>
T &List<T>::iterator::operator*() {
	return const_iterator::retrieve();
}

template<typename T>
T *List<T>::iterator::operator->() {
	return &const_iterator::retrieve();
}

template<typename T>
typename List<T>::iterator &List<T>::iterator::operator++() {
	const_iterator::operator++();
	return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator++(int) {
	iterator old = *this;
	++(*this);
	return old;
}

template<typename T>
typename List<T>::iterator &List<T>::iterator::operator--() {
	const_iterator::operator--();
	return *this;
}

template<typename T>
typename List<T>::iterator List<T>::iterator::operator--(int) {
	iterator old = *this;
	--(*this);
	return old;
}

// List class member function implementations

template<typename T>
List<T>::List() {
	init();
}

template<typename T>
List<T>::List(const List &rhs) {
	init();
	for (auto &x : rhs) {
		push_back(x);
	}
}

template<typename T>
List<T>::List(List &&rhs) : theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail} {
	rhs.theSize = 0;
	rhs.head = nullptr;
	rhs.tail = nullptr;
}

template<typename T>
List<T>::List(int num, const T &val) {
	init();
	for (int i = 0; i < num; ++i) {
		push_back(val);
	}
}

template<typename T>
List<T>::List(const_iterator start, const_iterator end) {
	init();
	while (start != end) {
		push_back(*start);
		++start;
	}
}

template<typename T>
void List<T>::print(std::ostream& os, char delim) const {
    for (auto itr = begin(); itr != end(); ++itr) {
        os << *itr;  // Output the element
        if (itr != --end()) {  // Don't add delimiter after the last element
            os << delim;
        }
    }
}


template<typename T>
List<T>::~List() {
	clear();
	delete head;
	delete tail;
}

// Copy Assignment Operator
template<typename T>
const List<T> &List<T>::operator=(const List &rhs) {
	List copy = rhs;
	std::swap(*this, copy);
	return *this;
}

// Move Assignment Operator

template<typename T>
List<T> &List<T>::operator=(List &&rhs) {
	std::swap(theSize, rhs.theSize);
	std::swap(head, rhs.head);
	std::swap(tail, rhs.tail);
	return *this;
}

template<typename T>
int List<T>::size() const {
	return theSize;
}

template<typename T>
bool List<T>::empty() const {
	return size() == 0;
}

template<typename T>
void List<T>::clear() {
	while (!empty()) {
		pop_front();
	}
}

template<typename T>
void List<T>::reverse() {
	if (empty()) return;
	auto first = begin();
	auto last = --end();
	while (first != last && first != ++last) {
		std::swap(*first, *last);
		++first;
		--last;
	}
}

template<typename T>
T &List<T>::front() {
	return *begin();
}

template<typename T>
T &List<T>::back() {
	return *(--end());
}

template<typename T>
void List<T>::push_front(const T &val) {
	insert(begin(), val);
}

template<typename T>
void List<T>::push_back(const T &val) {
	insert(end(), val);
}

template<typename T>
void List<T>::push_back(T&& val) {
    insert(end(), std::move(val));
}

template<typename T>
void List<T>::pop_front() {
	erase(begin());
}

template<typename T>
void List<T>::pop_back() {
	erase(--end());
}

// UNDEFINED 1

template<typename T>
typename List<T>::const_iterator List<T>::begin() const {
    return const_iterator(head->next);  // Return const_iterator starting at the first element
}

// UNDEFINED 2
template<typename T>
typename List<T>::const_iterator List<T>::end() const {
    return const_iterator(tail);  // Return const_iterator pointing to the tail (end marker)
}

// UNDEFINED 3

template<typename T>
typename List<T>::iterator List<T>::insert(iterator itr, T&& val) {
    Node* p = itr.current;  // Get the current node from the iterator
    theSize++;              // Increment the size of the list

    // Create a new node to hold the value
    Node* newNode = new Node(std::forward<T>(val), p->prev, p); // Use perfect forwarding
    p->prev->next = newNode; // Link the previous node to the new node
    p->prev = newNode;        // Link the new node to the current node

    return iterator(newNode); // Return an iterator to the newly inserted node
}


template<typename T>
void List<T>::remove(const T &val) {
	for (auto itr = begin(); itr != end(); ) {
		if (*itr == val) {
			itr = erase(itr);
		} else {
			++itr;
		}
	}
}

template<typename T>
typename List<T>::iterator List<T>::begin() {
	return iterator(head->next);
}

template<typename T>
typename List<T>::iterator List<T>::end() {
	return iterator(tail);
}

template<typename T>
typename List<T>::iterator List<T>::insert(iterator itr, const T &val) {
	Node *p = itr.current;
	theSize++;
	return iterator(p->prev = p->prev->next = new Node(val, p->prev, p));
}

template<typename T>
typename List<T>::iterator List<T>::erase(iterator itr) {
	Node *p = itr.current;
	iterator retVal(p->next);
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
	theSize--;
	return retVal;
}

template<typename T>
typename List<T>::iterator List<T>::erase(iterator start, iterator end) {
	while (start != end) {
		start = erase(start);
	}
	return end;
}

template<typename T>
void List<T>::init() {
	theSize = 0;
	head = new Node;
	tail = new Node;
	head->next = tail;
	tail->prev = head;
}

// Overloading equality operator
template<typename T>
bool operator==(const List<T>& lhs, const List<T>& rhs) {
    // Check if the sizes of both lists are equal
    if (lhs.size() != rhs.size()) {
        return false;
    }

    // Use iterators to compare elements at corresponding positions
    auto lhsItr = lhs.begin();
    auto rhsItr = rhs.begin();
    
    while (lhsItr != lhs.end() && rhsItr != rhs.end()) {
        if (*lhsItr != *rhsItr) {
            return false;
        }
        ++lhsItr;
        ++rhsItr;
    }

    // If all elements match, return true
    return true;
}

// Overloading inequality operator
template<typename T>
bool operator!=(const List<T>& lhs, const List<T>& rhs) {
    return !(lhs == rhs); // Reuse operator==
}

// UNDEFINED 4

// Overloading output stream operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& l) {
    // Use the List's own print method to output elements
    l.print(os);
    return os;
}

#endif
