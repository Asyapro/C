template<class T>
Vector<T>::Vector()
{
	capacity_ = 0;
	size_ = 0;
	data_ = 0;
}

template<class T>
Vector<T>::Vector(const Vector<T> & v)
{
	size_ = v.size_;
	capacity_ = v.capacity_;
	data_ = new T[size_];
	for (unsigned int i = 0; i < size_; i++)
		data_[i] = v.data_[i];
}

template<class T>
Vector<T>::Vector(unsigned int size)
{
	capacity_ = size;
	size_ = size;
	data_ = new T[size];
}

template<class T>
Vector<T>::Vector(unsigned int size, const T & initial)
{
	size_ = size;
	capacity_ = size;
	data_ = new T[size];
	for (unsigned int i = 0; i < size; i++)
		data_[i] = initial;
	//T();
}

template<class T>
Vector<T> & Vector<T>::operator= (const Vector<T> & v)
{
	delete[] data_;
	size_ = v.size_;
	capacity_ = v.capacity_;
	data_ = new T[size_];
	for (unsigned int i = 0; i < size_; i++)
		data_[i] = v.data_[i];
	return *this;
}

template<class T>
typename Vector<T>::iterator Vector<T>::begin()
{
	return data_;
}

template<class T>
typename Vector<T>::iterator Vector<T>::end()
{
	return data_ + size();
}

template<class T>
T& Vector<T>::front()
{
	return data_[0];
}

template<class T>
T& Vector<T>::back()
{
	return data_[size_ - 1];
}

template<class T>
void Vector<T>::push_back(const T & v)
{
	if (size_ >= capacity_)
		reserve(capacity_ + 5);
	data_[size_++] = v;
}

template<class T>
void Vector<T>::pop_back()
{
	size_--;
}

template<class T>
void Vector<T>::reserve(unsigned int capacity)
{
	if (data_ == 0)
	{
		size_ = 0;
		capacity_ = 0;
	}
	T * Newdata_ = new T[capacity];
	//assert(Newdata_);
	unsigned int l_Size = capacity < size_ ? capacity : size_;
	//copy (data_, data_ + l_Size, Newdata_);

	for (unsigned int i = 0; i < l_Size; i++)
		Newdata_[i] = data_[i];

	capacity_ = capacity;
	delete[] data_;
	data_ = Newdata_;
}

template<class T>
unsigned int Vector<T>::size()const//
{
	return size_;
}

template<class T>
void Vector<T>::resize(unsigned int size)
{
	reserve(size);
	size_ = size;
}

template<class T>
T& Vector<T>::operator[](unsigned int index)
{
	return data_[index];
}

template<class T>
unsigned int Vector<T>::capacity()const
{
	return capacity_;
}

template<class T>
Vector<T>::~Vector()
{
	delete[] data_;
}
template <class T>
void Vector<T>::clear()
{
	capacity_ = 0;
	size_ = 0;
	data_ = 0;
}

template <typename T>
void Vector<T>::toString() {
	std::lock_guard<std::mutex> lock(mutex_v_tostring);
	std::cout << std::endl << "Vector size = " << size_ << std::endl;
	for (unsigned long long i = 0; i < size_; ++i)
		std::cout << data_[i] << ' ';
	std::cout << std::endl;
};
