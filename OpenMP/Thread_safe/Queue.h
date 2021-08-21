#pragma once
#include <mutex>

template <typename T>

class Queue
{
public:
	Queue() = default;
	Queue(const Queue &q);
	~Queue() { std::lock_guard<std::mutex> lock(mutex_q_dest); while (!empty()) pop();}
	Queue& operator=(const Queue &q); 
	T &top(); 
	bool empty() { std::lock_guard<std::mutex> lock(mutex_q_empty); return first_ == nullptr; }
	void push(const T val);
	void pop();
	int size();
	clock_t CheckConnection() 
	{
		std::unique_lock<std::mutex> lock(mutex_q_connection);
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
		Node* prev_{ nullptr };
		T data_{ 0 };
		Node(Node* prev, const T &data) : prev_(prev), data_(data) { }
	};
	Node* first_{ nullptr };
	Node* last_{ nullptr };
	std::mutex mutex_q_cons, mutex_q_dest, mutex_q_equal, mutex_q_top, mutex_q_empty, mutex_q_push, mutex_q_pop, mutex_q_size, mutex_q_tostring, mutex_q_connection;
	bool state_unlock = false;
};

#include "Queue.tpp"