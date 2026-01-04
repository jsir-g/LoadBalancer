// RequestQueue.cpp
#include "RequestQueue.h"

RequestQueue::RequestQueue() : front_(nullptr), back_(nullptr), size_(0) {}

RequestQueue::~RequestQueue() {
    clear();
}

void RequestQueue::enqueue(const Request& r) {
    Node* n = new Node(r);
    if (!back_) {
        front_ = back_ = n;
    } else {
        back_->next = n;
        back_ = n;
    }
    ++size_;
}

Request RequestQueue::dequeue() {
    if (isEmpty()) {
        throw std::runtime_error("RequestQueue::dequeue() called on empty queue");
    }
    Node* old = front_;
    Request out = old->data;
    front_ = front_->next;
    if (!front_) back_ = nullptr;
    delete old;
    --size_;
    return out;
}

const Request& RequestQueue::front() const {
    if (isEmpty()) {
        throw std::runtime_error("RequestQueue::front() called on empty queue");
    }
    return front_->data;
}

bool RequestQueue::isEmpty() const {
    return size_ == 0;
}

std::size_t RequestQueue::size() const {
    return size_;
}

void RequestQueue::clear() {
    while (front_) {
        Node* n = front_;
        front_ = front_->next;
        delete n;
    }
    front_ = back_ = nullptr;
    size_ = 0;
}
