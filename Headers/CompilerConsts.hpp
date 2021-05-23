#pragma once

constexpr unsigned static_log2_constexpr_impl(unsigned value, short shift) {
    return shift == -1 ? -1 : 
        value & (1 << shift) ? shift :
            static_log2_constexpr_impl(value, shift - 1);
}
 
#define STATIC_LOG2(value)\
    (static_log2_constexpr_impl(value, sizeof(unsigned) * 8 - 1))

template <typename T, size_t n>
constexpr size_t array_size(const T(&)[n]) {
    return n;
}
