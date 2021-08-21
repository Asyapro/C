#include "pch.h"
#include "Stack.h"


template <typename T>
Stack<T>::Stack(const Stack& s) {
	std::lock_guard<std::mutex> lock(mutex_s_cons);
	head_ = nullptr;
	if (s.empty()) { return; }
	head_ = new Node(nullptr, s.head_->data_);
	Node* data = s.head_;
	Node* temp = head_;
	while (data->next_ != nullptr) {
		data = data->next_;
		temp->next_ = new Node(data->next_, data->data_);
		if (temp->next_ != nullptr)
			temp = temp->next_;
	}
};

template <typename T>
Stack<T>& Stack<T>:: operator=(const Stack<T>& s) {
	std::lock_guard<std::mutex> lock(mutex_s_equal);
	if (&s == this) return *this;
	Stack temp(s);
	Node* tmp = temp.head_;
	temp.head_ = head_;
	head_ = tmp;
	return *this;
};

template <typename T>
T& Stack<T>::top() {
	std::lock_guard<std::mutex> lock(mutex_s_top);
	if (empty())
		throw std::runtime_error("Stack is empty!");
	return head_->data_;
};

template <typename T>
void Stack<T>::push(const T val) {
	std::lock_guard<std::mutex> lock(mutex_s_push);
	head_ = new Node(head_, val);
};

template <typename T>
void Stack<T>::pop() {
	std::lock_guard<std::mutex> lock(mutex_s_pop);
	if (empty())
		throw std::runtime_error("Stack is empty!");
	Node* old = head_;
	head_ = head_->next_;
	delete old;
};

template <typename T>
int Stack<T>::size() {
	std::lock_guard<std::mutex> lock(mutex_s_size);
	int count = 0;
	Node* st = head_;
	while (head_ != nullptr) {
		++count;
		head_ = head_->next_;
	}
	head_ = st;
	return count;
};

template <typename T>
void Stack<T>::toString() {
	std::lock_guard<std::mutex> lock(mutex_s_tostring);
	Node* temp = head_;
	std::cout << std::endl;
	for (int i = 0; i < this->size(); ++i) {
		std::cout << temp->data_ << "  ";
		temp = temp->next_;
	}
	std::cout << std::endl;
};