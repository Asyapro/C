#pragma once
#include <mutex>

template <typename T>

class Vector
{
public:
	typedef T * iterator;

	Vector();
	Vector(unsigned int size);
	Vector(unsigned int size, const T & initial);
	Vector(const Vector<T> & v);
	~Vector();

	unsigned int capacity() const;
	unsigned int size() const;
	bool empty() const { return (size_ <= 0); }
	iterator begin();
	iterator end();
	T & front();
	T & back();
	void push_back(const T & value);
	void pop_back();

	void reserve(unsigned int capacity);
	void resize(unsigned int size);

	T & operator[](unsigned int index);
	Vector<T> & operator=(const Vector<T> &);
	void clear();
	clock_t CheckConnection() 
	{
		std::unique_lock<std::mutex> lock(mutex_v_connection);
		if (state_unlock) lock.unlock();
		clock_t time = clock();
		std::cout << std::endl << "thread id " << std::this_thread::get_id() << std::endl; 
		std::cout << "connection ok" << std::endl;
		return time;
	}
	void StateUnlock(bool state_unlock) { this->state_unlock = state_unlock; }
	void toString();

private:
	unsigned int size_;
	unsigned int capacity_;
	T * data_;
	std::mutex mutex_v_cons_val, mutex_v_cons, mutex_v_dest, mutex_v_equal, mutex_v_par, mutex_v_push_back, mutex_v_pop, mutex_v_size, mutex_v_tostring, mutex_v_connection;
	bool state_unlock = false;
};

#include "Vector.tpp"