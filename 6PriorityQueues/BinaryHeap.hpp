#pragma once

#include "../2Arrays/DynArray.hpp"

template <typename T>
class BinaryHeap : public DynArray<T> {
public:
    BinaryHeap() : DynArray<T>() {}

    virtual ~BinaryHeap() = default;
    
    static size_t parent(size_t i) noexcept {
        return (i - 1) / 2;
    }

    static size_t leftChild(size_t i) noexcept {
        return 2 * i + 1;
    }

    static size_t rightChild(size_t i) noexcept {
        return 2 * i + 2;
    }

    Maybe<T> poll() noexcept {
        if (!this->size()) {
            return Maybe<T>();
        }

        auto result = this->at(0);

        if (this->size() == 1) {
            this->removeAt(0);
            return result;
        }

        std::swap(this[0], this[this->size() - 1]);
        this->removeAt(this->size() - 1);

        if (this->size() == 1) {
            return result;
        }

        heapify(0);
        return result;
    }

    void add(T const& val) noexcept {
        this->add(val);
        // "Bubbling Up"
        for (size_t i = this->size() - 1; i > 0 && this[parent(i)] < this[i]; i = parent(i)) {
            std::swap(this[parent(i)], this[i]);
        }
    }

    // void insertAt(size_t index, T const& val) noexcept;
    // bool contains(T const& elem) const noexcept;
    // void removeAt(size_t index) noexcept;

private:
    // "Bubbling Down"
    void heapify(size_t i) {
        size_t left = leftChild(i);
        size_t right = rightChild(i);
        size_t largest = i;

        if (left < this->size() && this[left] > this[largest]) largest = left;
        if (right < this->size() && this[right] > this[largest]) largest = right;

        if (largest != i) {
            std::swap(this[i], this[largest]);
            heapify(largest);
        }
    }
};
