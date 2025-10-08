#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>

template <class T>
struct Node {
	Node(const T& value, Node<T>* prev) {
		this->value = value;
		this->prev = prev;
	}

	T value;
	Node<T>* prev;
};

template <class T>
class Stack {
protected:
	Node<T>* _top;
	size_t s;
public:
	Stack();
	~Stack();
	Stack(const Stack&) = delete;
	Stack& operator=(const Stack&) = delete;
	void push(T value);
	void pop();
	T top();
	const T& top() const;
	size_t size() const;
	bool empty() const;
	void clear();
};

template <class T>
Stack<T>::Stack() : _top(nullptr), s(0) {}

template <class T>
Stack<T>::~Stack() { clear(); }

template <class T>
void Stack<T>::push(T value) {
	Node<T>* elem = new Node<T>(value, _top);
	_top = elem;
	++s;
}

template <class T>
void Stack<T>::pop() {
	if (empty()) throw std::runtime_error("pop() called on empty stack");
	Node<T>* temp = _top;
	_top = _top->prev;
	delete temp;
	--s;
}

template <class T>
T Stack<T>::top() {
	if (empty()) throw std::runtime_error("front() called on empty stack");
	return _top->value;
}

template <class T>
const T& Stack<T>::top() const {
	if (empty()) throw std::runtime_error("top() called on empty stack");
	return _top->value;
}

template <class T>
size_t Stack<T>::size() const { return s; }

template <class T>
bool Stack<T>::empty() const { return !s; }

template <class T>
void Stack<T>::clear() {
	while (_top) {
		Node<T>* temp = _top;
		_top = _top->prev;
		delete temp;
	}
	s = 0;
	_top = nullptr;
}

#endif