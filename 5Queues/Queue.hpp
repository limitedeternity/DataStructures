#pragma once

#include "../3LinkedLists/DoublyLinkedList.hpp"

template <typename T>
class Queue : public DoublyLinkedList<T> {
public:
    Queue() : DoublyLinkedList<T>() {}

    virtual ~Queue() = default;

    Maybe<T> poll() noexcept {
        auto result = this->getFirst();
        this->removeFirst();
        return result;
    }

    void offer(T const& elem) noexcept {
        this->addLast(elem);
    }

    bool contains(T const& elem) const noexcept {
        return this->indexOf(elem).isJust();
    }
};
