#pragma once
#include <mutex>
template <typename T>
class Stack
{
public:
	Stack() = default;
	Stack(const Stack& st);
	~Stack() { std::unique_lock<std::mutex> lock(mutex_s_dest); while (!empty()) pop();}
	Stack& operator=(const Stack& st);
	T& top(); 
	bool empty() const { return head_ == nullptr; } //std::lock_guard<std::mutex> lock(mutex_s_empty);
	void push(const T val);
	void pop();
	int size();
	clock_t CheckConnection() 
	{
		std::unique_lock<std::mutex> lock(mutex_s_connection);
		if (state_unlock) lock.unlock();
		clock_t time = clock();
		std::cout << std::endl << "thread id " << std::this_thread::get_id() << std::endl; 
		std::cout << "connection ok" << std::endl;
		return time;
	}
	void StateUnlock(bool state_unlock) { this->state_unlock = state_unlock; }
	void toString();

private:
	struct Node {
		Node* next_{ nullptr };
		T data_{ 0 };
		Node(Node* next_, const T& data_) : next_(next_), data_(data_) {}
	};
	Node* head_{ nullptr };
	std::mutex mutex_s_cons, mutex_s_dest, mutex_s_equal, mutex_s_top, mutex_s_empty, mutex_s_push, mutex_s_pop, mutex_s_size, mutex_s_tostring, mutex_s_connection;
	bool state_unlock = false;
};

#include "Stack.tpp"