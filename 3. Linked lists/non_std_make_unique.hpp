#pragma once
#include <memory>

// std::make_unique polyfills for C++11
namespace non_std {
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
}

