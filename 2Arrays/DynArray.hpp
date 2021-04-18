#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "../Headers/Functional.hpp"
#include "../Headers/NonSTD.hpp"

template <typename T>
class DynArray {
public:
    DynArray(size_t size = 10) noexcept : 
        pa(non_std::make_unique<T[]>(size)),
        length{size},
        nextIndex{0}
    {}

    DynArray(const DynArray& source) noexcept :
        pa(non_std::make_unique<T[]>(source.length)),
        length{source.length},
        nextIndex{source.nextIndex}
    {
        std::memcpy(pa.get(), source.pa.get(), sizeof(T) * source.length);
    }

    DynArray& operator=(const DynArray& source) noexcept {
        if (std::addressof(*this) != std::addressof(source)) {
            pa = non_std::make_unique<T[]>(source.length);
            length = source.length;
            nextIndex = source.nextIndex;
            std::memcpy(pa.get(), source.pa.get(), sizeof(T) * source.length);
        }

        return *this;
    }

    DynArray(DynArray&& source) noexcept :
        length{source.length},
        nextIndex{source.nextIndex}
    {
        pa.reset(source.pa.release());
    }

    DynArray& operator=(DynArray&& source) noexcept {
        if (std::addressof(*this) != std::addressof(&source)) {
            pa.reset(source.pa.release());
            length = source.length;
            nextIndex = source.nextIndex;
        }

        return *this;
    }

    T& operator[](size_t index) {
        if (nextIndex == 0 || index >= nextIndex) {
            throw std::out_of_range("Out of range");
        }

        return pa[index];
    }

    Maybe<T> at(size_t index) const noexcept {
        if (nextIndex == 0) {
            return Maybe<T>();
        }

        return return_<Maybe>(pa[index >= nextIndex ? nextIndex - 1 : index]);
    }

    void add(T const& val) noexcept {
        if (nextIndex == length) {
            if (length == 0) length = 1;
            else length *= 2;

            auto prev = pa.release();
            pa = non_std::make_unique<T[]>(length);
            std::memcpy(pa.get(), prev, sizeof(T) * nextIndex);

            delete[] prev;
        }

        pa[nextIndex++] = val;
    }

    void insertAt(size_t index, T const& val) noexcept {
        if (index >= nextIndex) return add(val);

        auto prev = pa.release();
        pa = non_std::make_unique<T[]>(length + 1);

        if (index > 0) std::memcpy(pa.get(), prev, sizeof(T) * index);
        pa[index] = val;
        std::memcpy(pa.get() + index + 1, prev + index, sizeof(T) * (nextIndex - index));

        delete[] prev;
        length++;
        nextIndex++;
    }

    void removeAt(size_t index) noexcept {
        if (nextIndex == 0) return;
        if (index >= nextIndex) index = nextIndex - 1;

        auto prev = pa.release();
        pa = non_std::make_unique<T[]>(length - 1);

        if (index > 0) std::memcpy(pa.get(), prev, sizeof(T) * index);
        std::memcpy(pa.get() + index, prev + index + 1, sizeof(T) * (nextIndex - index));

        delete[] prev;
        length--;
        nextIndex--;
    }

    void clear() noexcept {
        pa = non_std::make_unique<T[]>(10);
        length = 10;
        nextIndex = 0;
    }

    size_t size() const noexcept {
        return nextIndex;
    }

    friend std::string to_string(DynArray const& d) noexcept {
        std::stringstream ss;
        ss << "[";
        if (d.nextIndex == 0) {
            ss << "]";
            return ss.str();
        }

        for (size_t i = 0; i < d.nextIndex - 1; ++i) {
            ss << non_std::to_string(d.at(i).fromJust()) << ", ";
        }

        ss << non_std::to_string(d.at(d.nextIndex - 1).fromJust()) << "]";
        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, DynArray const& d) noexcept {
        return os << non_std::to_string(d); 
    }

    bool operator!=(DynArray const& rhs) const noexcept {
        return !operator==(rhs);
    }

    bool operator==(DynArray const& rhs) const noexcept {
        if (std::addressof(*this) == std::addressof(rhs)) {
            return true;
        }

        if (nextIndex != rhs.nextIndex) {
            return false;
        }

        for (size_t i = 0; i < nextIndex; ++i) {
            if (at(i) != rhs.at(i)) {
                return false;
            }
        }

        return true;
    }

private:
    std::unique_ptr<T[]> pa;
    size_t length;
    size_t nextIndex;
};

int main(void) {
    DynArray<int> d(3);
    d.add(1);
    d.add(2);
    d.add(3);
    d.add(4);
    d.add(5);

    std::cout << d << std::endl;

    d.removeAt(4);
    d.removeAt(1);
    d.add(50);

    std::cout << d << std::endl;

    d.insertAt(1, 9);
    d.insertAt(0, 10);
    d.add(25);

    std::cout << d << std::endl;
    return 0;
}
