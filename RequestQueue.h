// RequestQueue.h
#pragma once
#include "Request.h"
#include <cstddef>
#include <stdexcept>

/**
 * @file RequestQueue.h
 * @brief Linked-list queue implementation for Requests.
 */
class RequestQueue {
public:
    RequestQueue();
    ~RequestQueue();

    RequestQueue(const RequestQueue&) = delete;
    RequestQueue& operator=(const RequestQueue&) = delete;

    /**
     * @brief Add request to back of queue.
     */
    void enqueue(const Request& r);

    /**
     * @brief Remove and return front request.
     * @throws std::runtime_error if empty.
     */
    Request dequeue();

    /**
     * @brief Peek front request (const).
     * @throws std::runtime_error if empty.
     */
    const Request& front() const;

    bool isEmpty() const;
    std::size_t size() const;

private:
    struct Node {
        Request data;
        Node* next;
        explicit Node(const Request& r) : data(r), next(nullptr) {}
    };

    Node* front_;
    Node* back_;
    std::size_t size_;

    void clear();
};
