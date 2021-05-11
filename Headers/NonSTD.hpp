#pragma once
#include <memory>
#include <string>

namespace non_std {
    // ---- std::make_unique polyfills for C++11

    // Constructable objects
    template<typename T, typename... Args>
    typename std::enable_if<std::is_constructible<T, Args...>::value, std::unique_ptr<T>>::type
    make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    // Structures brace init
    template<typename T, typename... Args>
    typename std::enable_if<!std::is_constructible<T, Args...>::value, std::unique_ptr<T>>::type
    make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T{std::forward<Args>(args)...});
    }

    // Arrays
    template<typename T>
    typename std::enable_if<std::is_array<T>::value, std::unique_ptr<T>>::type
    make_unique(size_t size) {
        return std::unique_ptr<T>(new typename std::remove_extent<T>::type[size]);
    }

    // ---- Support for std::to_string overloading
    namespace adl_helper {
        using std::to_string;

        template<typename T>
        std::string as_string(T&& t) {
            return to_string(std::forward<T>(t));
        }
    }

    template<typename T>
    std::string to_string(T&& t) {
        return adl_helper::as_string(std::forward<T>(t));
    }

    template<typename T>
    struct has_meaningless_destructor :
        std::integral_constant<
            bool,
            std::is_arithmetic<T>::value ||
            std::is_enum<T>::value ||
            std::is_member_pointer<T>::value ||
            std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value ||
            std::is_reference<T>::value ||
            std::is_void<T>::value
        >
    {};
}
