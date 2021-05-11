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
    DynArray(size_t size = 10) {
        internal_reserve(!size ? 1 : size);
    }

    virtual ~DynArray() noexcept {
        free(m_buffer);
    }

    DynArray(const DynArray& source) {
        if (!source.size()) {
            internal_reserve(1);
        }
        else {
            append(source.data(), source.size());
        }
    }

    DynArray& operator=(const DynArray& source) {
        if (std::addressof(*this) != std::addressof(source)) {
            clear();
            append(source.data(), source.size());
        }

        return *this;
    }

    DynArray(DynArray&& source) noexcept {
        if (!source.size()) {
            internal_reserve(1);
        }
        else {
            append(source.data(), source.size(), true);
        }
    }

    DynArray& operator=(DynArray&& source) noexcept {
        clear();
        append(source.data(), source.size(), true);
        return *this;
    }

    void reserve(size_t reserve_size) {
        if (reserve_size > m_capacity) {
            internal_reserve(reserve_size);
        }
    }

    void resize(size_t new_len) {
        if (new_len > m_capacity) {
            internal_reserve(new_len);
        }

        m_size = new_len;
    }

    T* data() const noexcept {
        return m_buffer;
    }

    T& operator[](size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("Out of range");
        }

        return m_buffer[index];
    }

    Maybe<T> at(size_t index) const noexcept {
        if (index >= m_size) {
            return Maybe<T>();
        }

        return return_<Maybe>(m_buffer[index]);
    }

    void add(T const& val) {
        resize(m_size + 1);
        m_buffer[m_size - 1] = val;
    }

    void append(const T* data, size_t len, bool move_allowed = false) {
        if (!len) return;

        uint64_t this_range_start = (uintptr_t)m_buffer;
        uint64_t this_range_end = this_range_start + m_size;
        uint64_t buffer_range_start = (uintptr_t)data;
        uint64_t buffer_range_end = buffer_range_start + len;

        if (this_range_end >= buffer_range_start &&
            this_range_start <= buffer_range_end) {

            return;
        }

        size_t pos = m_size;
        size_t new_pos = pos + len;
        resize(new_pos);

        if (move_allowed) {
            memmove(m_buffer + pos, data, len);
        }
        else {
            memcpy(m_buffer + pos, data, len);
        }
    }

    void insertAt(size_t index, T const& val) noexcept {
        if (index >= m_size) return add(val);

        resize(m_size + 1);
        memmove(m_buffer + index + 1, m_buffer + index, sizeof(T) * (m_size - index - 1));
        m_buffer[index] = val;
    }

    void removeAt(size_t index) noexcept {
        if (!m_size) return;
        if (index >= m_size) index = m_size - 1;

        #if __cplusplus > 201402L
        if constexpr (!non_std::has_meaningless_destructor<T>::value)
        #else
        if (!non_std::has_meaningless_destructor<T>::value)
        #endif
            m_buffer[index].~T();

        if (index < m_size - 1)
            memmove(m_buffer + index, m_buffer + index + 1, sizeof(T) * (m_size - index - 1));

        m_size--;
    }

    void clear() noexcept {
        #if __cplusplus > 201402L
        if constexpr (!non_std::has_meaningless_destructor<T>::value) {
        #else
        if (!non_std::has_meaningless_destructor<T>::value) {
        #endif
            for (size_t i = 0; i < m_size; ++i) {
                m_buffer[i].~T();
            }
        }

        m_size = 0;
    }

    size_t size() const noexcept {
        return m_size;
    }

    friend std::string to_string(DynArray const& d) noexcept {
        std::stringstream ss;
        ss << "[";
        if (!d.m_size) {
            ss << "]";
            return ss.str();
        }

        for (size_t i = 0; i < d.m_size - 1; ++i) {
            ss << non_std::to_string(d.at(i).fromJust()) << ", ";
        }

        ss << non_std::to_string(d.at(d.m_size - 1).fromJust()) << "]";
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

        if (m_size != rhs.m_size) {
            return false;
        }

        for (size_t i = 0; i < m_size; ++i) {
            if (at(i) != rhs.at(i)) {
                return false;
            }
        }

        return true;
    }

private:
    void internal_reserve(size_t reserve_size) {
        T* new_buffer = (T*)realloc(m_buffer, reserve_size * sizeof(T));
        if (new_buffer == nullptr) {
            throw std::bad_alloc();
        }

        m_capacity = reserve_size;
        m_buffer = new_buffer;
    }

    size_t m_size = 0;
    size_t m_capacity = 0;
    T* m_buffer = nullptr;
};

/*int main(void) {
    DynArray<int> d(3);
    d.add(1);
    d.add(2);
    d.add(3);
    d.add(4);
    d.add(5);

    std::cout << d << std::endl;

    d.removeAt(4);
    d.removeAt(0);
    d.add(50);

    std::cout << d << std::endl;

    d.insertAt(1, 9);
    d.insertAt(0, 10);
    d.add(25);

    std::cout << d << std::endl;
    return 0;
}*/
