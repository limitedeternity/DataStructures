#pragma once

#include "../2Arrays/DynArray.hpp"

template <typename T>
class MaxPQ : public DynArray<T> {
public:
    MaxPQ() : DynArray<T>() {}

    virtual ~MaxPQ() = default;

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

        std::swap(this->operator[](0), this->operator[](this->size() - 1));
        this->removeAt(this->size() - 1);

        if (this->size() == 1) {
            return result;
        }

        heapify(0);
        return result;
    }

    void offer(T const& val) noexcept {
        this->add(val);

        // "Bubbling Up"
        for (size_t i = this->size() - 1; i > 0 && this->at(parent(i)).fromJust() < this->at(i).fromJust(); i = parent(i)) {
            std::swap(this->operator[](parent(i)), this->operator[](i));
        }
    }

    bool contains(T const& elem) const noexcept {
        if (!this->size() || this->at(0).fromJust() < elem) {
            return false;
        }

        return fix(
            [&](auto f, size_t i) -> bool {
                return (this->at(i).fromJust() == elem) || (
                        (i < this->size()) && (
                            (
                                this->at(leftChild(i)).on(
                                   [&](T const& val) {
                                       return val >= elem;
                                   },
                                   []() {
                                       return false;
                                   }
                                ) ? f(leftChild(i)) : false
                            ) ||
                            (
                                this->at(rightChild(i)).on(
                                   [&](T const& val) {
                                       return val >= elem;
                                   },
                                   []() {
                                       return false;
                                   }
                                ) ? f(rightChild(i)) : false
                            )
                        )
                );
            }
        )(0);
    }

private:
    // "Bubbling Down"
    void heapify(size_t i) noexcept {
        size_t left = leftChild(i);
        size_t right = rightChild(i);
        size_t largest = i;

        if (left < this->size() && this->operator[](left) > this->operator[](largest)) largest = left;
        if (right < this->size() && this->operator[](right) > this->operator[](largest)) largest = right;

        if (largest != i) {
            std::swap(this->operator[](i), this->operator[](largest));
            heapify(largest);
        }
    }
};

/*int main(void) {
    MaxPQ<int> pq;

    pq.offer(1);
    pq.offer(10);
    pq.offer(9);
    pq.offer(5);
    pq.offer(15);

    std::cout << (pq.contains(1) ? "+" : "-") << std::endl;
    std::cout << (pq.contains(9) ? "+" : "-") << std::endl;
    std::cout << (pq.contains(5) ? "+" : "-") << std::endl;
    std::cout << (pq.contains(10) ? "+" : "-") << std::endl;
    std::cout << (pq.contains(15) ? "+" : "-") << std::endl;
    std::cout << (pq.contains(12) ? "+" : "-") << std::endl;

    size_t sz = pq.size();
    for (size_t i = 0; i < sz; ++i) {
        std::cout << pq.poll().fromJust() << std::endl;
    }
}*/
