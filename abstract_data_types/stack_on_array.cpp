#include "stackonarray.h"
#include <iostream>
#include <utility>

StackOnArray::StackOnArray(const StackOnArray &arr) {
    capacity_ = arr.capacity_;
    head_ = arr.head_;
    data_ = new int[capacity_];
    for (int i = 0; i <= arr.head_; i++) {
        data_[i] = arr.data_[i];
    }
}

void StackOnArray::push(const int val) {
    if (capacity_ <= head_ + 1) {
        if (capacity_ == 0) {
            capacity_ = 1;
        } else {
            capacity_ = capacity_ * 2;
        }
        int *data = new int[capacity_];
        for (int i = 0; i <= head_; i++) {
            data[i] = data_[i];
        }
        data_ = data;
    }
    head_++;
    data_[head_] = val;
}

int& StackOnArray::top() {
    if (empty()) {
        throw std::runtime_error("empty!");
    }
    return data_[head_];
}

void StackOnArray::pop() {
    if (empty()) {
        throw std::runtime_error("empty!");
    }
    head_--;
}

StackOnArray& StackOnArray::operator=(const StackOnArray &arr) {
    if (&arr == this) { return *this; }
    StackOnArray temp(arr);
    std::swap(temp.data_, data_);
    std::swap(temp.head_, head_);
    std::swap(temp.capacity_, capacity_);
    return (*this);
}
