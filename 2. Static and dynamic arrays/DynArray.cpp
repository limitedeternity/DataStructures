#include <iostream>
#include <memory>

template <class T>
class DynArray {
public:
    DynArray() {
        this(10);
    }

    DynArray(size_t size) : pa{new T[size]} {
        length = size;
        nextIndex = 0;
    }

    // copy constructors
    DynArray(const DynArray&) = delete;
    DynArray& operator=(const DynArray&) = delete;

    // move constructors
    DynArray(DynArray&& source) noexcept {
        pa.reset(source.pa.release());
        length = source.length;
        nextIndex = source.nextIndex;
    }

    DynArray& operator=(DynArray&& source) noexcept {
        if (this != &source) {
            pa.reset(source.pa.release());
            length = source.length;
            nextIndex = source.nextIndex;
        }

        return *this;
    }

    T& operator[](size_t index) {
        // Нафиг undefined behavior
        if (index >= nextIndex) {
            throw std::out_of_range("Out of range");
        }

        return pa[index];
    }

    T& at(size_t index) {
        if (index >= nextIndex) {
            throw std::out_of_range("Out of range");
        }

        return pa[index];
    }

    void add(T const& val) {
        if (nextIndex == length) {
            if (length == 0) length = 1;
            else length *= 2;

            auto prev = pa.release();
            pa.reset(new T[length]);

            for (size_t i = 0; i < nextIndex; ++i) {
                pa[i] = prev[i];
            }

            delete[] prev;
        }

        pa[nextIndex++] = val;
    }

    void insertAt(size_t index, T const& val) {
        if (index >= nextIndex) {
            throw std::out_of_range("Out of range");
        }

        auto prev = pa.release();
        pa.reset(new T[length + 1]);

        for (size_t i = 0; i < index; ++i) {
            pa[i] = prev[i];
        }

        pa[index] = val;

        for (size_t i = index; i < nextIndex; ++i) {
            pa[i + 1] = prev[i];
        }

        delete[] prev;
        length++;
        nextIndex++;
    }

    void removeAt(size_t index) {
        if (index >= nextIndex) {
            throw std::out_of_range("Out of range");
        }

        auto prev = pa.release();
        pa.reset(new T[length - 1]);

        for (size_t i = 0; i < index; ++i) {
            pa[i] = prev[i];
        }

        for (size_t i = index + 1; i < nextIndex; ++i) {
            pa[i - 1] = prev[i];
        }

        delete[] prev;
        length--;
        nextIndex--;
    }

    void clear() {
        pa.reset(new T[10]);
        length = 10;
        nextIndex = 0;
    }

    size_t size() noexcept {
        return nextIndex;
    }

    friend std::ostream& operator<<(std::ostream& os, DynArray const& d) {
        os << "[";
        for (size_t i = 0; i < d.size() - 1; ++i) {
            os << d[i] << ", ";
        }

        os << d[d.size() - 1] << "]";
        return os; 
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
