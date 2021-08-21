#include "stackonlist.h"
#include <iostream>

StackOnList::StackOnList(const StackOnList &arr) {
    head_ = nullptr;
    if (arr.empty()) { return; }
    head_ = new Node(nullptr, arr.head_->data_);
    Node *data = arr.head_;
    Node *current = head_;
    while (data->next_ != nullptr) {
        data = data->next_;
        current->next_ = new Node(data->next_, data->data_);
        if (current->next_ != nullptr) {
            current = current->next_;
        }
    }
}

StackOnList::~StackOnList() {
    while (!empty()) pop();
}

void StackOnList::push(const int val) {
    head_ = new Node(head_, val);
}

int& StackOnList::top() {
    if (empty()) {
        throw std::runtime_error("empty!");
    }
    return head_->data_;
}

void StackOnList::pop() {
    if (empty()) {
        throw std::runtime_error("empty!");
    }
    Node *old = head_;
    head_ = head_->next_;
    delete old;
}

StackOnList& StackOnList::operator=(const StackOnList &arr) {
    if (&arr == this) { return *this; }
    StackOnList temp(arr);
    Node* tmp = temp.head_;
    temp.head_ = head_;
    head_ = tmp;
    return (*this);
}
