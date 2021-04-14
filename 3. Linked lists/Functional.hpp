#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include "non_std_make_unique.hpp"

using std::function;
using std::vector;

template <template <typename...> class F>
struct Functor {
    template <typename A, typename B>
    static function <F<B>(F<A>)> fmap(function <B(A)>);
};

template <template <typename...> class F, typename A, typename B>
static function <F<B>(F<A>)> fmap(function <B(A)> f) {
    return Functor<F>::fmap(f);
}

template <template <typename...> class F, typename A, typename B>
static F<B> fmap_(function <B(A)> f, F<A> const& v) {
    return Functor<F>::fmap(f)(v);
}

template <template <typename...> class F, typename A, typename B>
static F<B> operator %(function <B(A)> f, F<A> const& v) {
    return Functor<F>::fmap(f)(v);
}

template <template <typename...> class F>
struct Monad {
    template <typename A>
    static F<A> return_(A const&);

    template <typename A, typename B>
    static F<B> bind(F<A> const&, function<F<B>(A)>);
};

template <template <typename...> class F, typename A, typename B>
static F<B> bind(F<A> const& m, function<F<B>(A)> f) {
    return Monad<F>::bind(m, f);
}

template <template <typename...> class F, typename A>
static F<A> return_(A const& a) {
    return Monad<F>::return_(a);
}

template <template <typename...> class F, typename A, typename B>
static F<B> operator>=(F<A> const& m, function<F<B>(A)> f) {
    return Monad<F>::bind(m, f);
}

template <template <typename...> class F, typename A, typename B>
static F<B> operator>>(F<A> const& a, F<B> const& b) {
    function<F<B>(A)> f = [b](A) { return b; };
    return a >= f;
}

template <typename T>
class Maybe {
public:
    Maybe() {};
    explicit Maybe(T const& value) : _value(std::move(non_std::make_unique<T>(value))) {};

    template <typename WhenJust, typename WhenNothing>
    auto on(WhenJust&& whenJust, WhenNothing&& whenNothing) -> decltype(whenNothing()) {
        if (isNothing()) return whenNothing();
        return whenJust(*_value);
    }

    T& fromJust() const {
        if (isJust()) {
            return *_value;
        }

        throw std::invalid_argument("Cannot get value from Nothing");
    }

    bool isJust() const { return _value.get() != nullptr; }
    bool isNothing() const { return _value.get() == nullptr; }

    static bool isJust(Maybe& m) { return m.isJust(); }
    static bool isNothing(Maybe& m) { return m.isNothing(); }

    friend std::ostream& operator<<(std::ostream& os, Maybe<T> const& m) {
        if (m.isJust()) {
            return os << "Just " << m.fromJust();
        }

        return os << "Nothing";
    }

private:
    std::shared_ptr<T> _value = nullptr;
};

template <>
struct Functor<Maybe> {
    template <typename A, typename B>
    static function <Maybe<B>(Maybe<A>)> fmap(function <B(A)> f) {
        return [f](Maybe<A> m) -> Maybe<B> {
            if (m.isNothing()) {
                return Maybe<B>();
            } else {
                return Maybe<B>(f(m.fromJust()));
            }
        };
    };
};

template <>
struct Monad<Maybe> {
    template <typename A>
    static Maybe<A> return_(A const& v) {
        return Maybe<A>(v);
    }

    template <typename A, typename B>
    static Maybe<B> bind(Maybe<A> const& m, function<Maybe<B>(A)> f) {
        if (m.isNothing()) {
            return Maybe<B>();
        }

        return f(m.fromJust());
    }
};

template <>
struct Functor<vector> {
    template <typename A, typename B>
    static function <vector<B>(vector<A>)> fmap(function <B(A)> f) {
        return [f](vector<A> v) {
            vector<B> result;
            result.reserve(v.size());

            std::transform(v.begin(), v.end(), std::back_inserter(result), f);
            return result;
        };
    }
};

template <typename A, typename B, typename C>
static function<C(A)> compose(function<B(A)> f1, function<C(B)> f2) {
    return [f1, f2](A v) -> C {
        return f2(f1(v));
    };
}
