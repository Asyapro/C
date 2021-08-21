#include "pch.h"
#include "Queue.h"

template <typename T>
Queue<T>::Queue(const Queue<T> &q) {
	std::lock_guard<std::mutex> lock(mutex_q_cons);
	first_ = nullptr;
	if (q.empty()) return;
	first_ = new Node(nullptr, q.first_->data_);
	last_ = first_;
	Node* data = q.first_;
	while (data != q.last_) {
		data = data->next_;
		last_->next_ = new Node(last_, data->data_);
		last_->next_->prev_ = last_;
		last_ = last_->next_;
	}
};

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T> &q) {
	std::lock_guard<std::mutex> lock(mutex_q_equal);
	if (&q == this) {
		return *this;
	}

	Queue temp(q);
	Node* tmp = temp.last_;
	temp.last_ = last_;
	last_ = tmp;
	return (*this);
};

template <typename T>
T& Queue<T>::top() {
	if (empty()) {
		throw std::runtime_error("Queue is empty");
	}
	return first_->data_;
};

template <typename T>
void Queue<T>::push(const T val) {
	std::lock_guard<std::mutex> lock(mutex_q_push);
	if (empty()) {
		last_ = new Node(nullptr, val);
		first_ = last_;
	}
	else {
		last_->next_ = new Node(last_, val);
		last_->next_->prev_ = last_;
		last_ = last_->next_;
	}
};

template <typename T>
void Queue<T>::pop() {
	std::lock_guard<std::mutex> lock(mutex_q_pop);
	if (empty())
		throw std::runtime_error("Queue is empty!");
	Node* old = first_;
	first_ = first_->next_;
	delete old;
};

template <typename T>
int Queue<T>::size() {
	std::lock_guard<std::mutex> lock(mutex_q_size);
	Node* temp = first_;
	int count = 0;
	while (temp != nullptr) {
		++count;
		temp = temp->next_;
	}
	return count;
};

template <typename T>
void Queue<T>::toString() {
	std::lock_guard<std::mutex> lock(mutex_q_tostring);
	Node* temp = first_;
	std::cout << std::endl;
	for (int i = 0; i < this->size(); ++i) {
		std::cout << temp->data_ << "  ";
		temp = temp->next_;
	}
	std::cout << std::endl;
};