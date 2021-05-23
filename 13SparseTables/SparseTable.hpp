#pragma once

#include <iostream>
#include <functional>
#include "../Headers/CompilerConsts.hpp"
 
enum class STOperation {
    MIN,
    MAX,
    SUM,
    MUL,
    GCD
};

template <size_t n>
class SparseTable {
    static_assert(n > 0, "Size must be natural");

    // The maximum power of 2 needed. This value is floor(log2(n)) 
    static constexpr const unsigned P = STATIC_LOG2(n);

public:
    SparseTable(long (&values)[n], STOperation operation) noexcept {
        op = operation;
        init(values);
    }

    virtual ~SparseTable() = default;

    long query(size_t l, size_t r) const noexcept {
        switch (op) {
            // O(1)
            case STOperation::MIN: return query(l, r, minFn);
            case STOperation::MAX: return query(l, r, maxFn);
            case STOperation::GCD: return query(l, r, gcdFn);

            // O(log2(n))
            case STOperation::SUM: return sumQuery(l, r);
            case STOperation::MUL: return mulQuery(l, r);
        }
    }

    size_t queryIndex(size_t l, size_t r) const {
        switch (op) {
            case STOperation::MIN: return minQueryIndex(l, r);
            case STOperation::MAX: return maxQueryIndex(l, r);
            default: throw std::invalid_argument("Unsupported operation type");
        }
    }

private:
    size_t minQueryIndex(size_t l, size_t r) const noexcept {
        size_t len = r - l + 1;
        unsigned p = log2[len];
        long leftInterval = dp[p][l];
        long rightInterval = dp[p][r - (1 << p) + 1];

        if (leftInterval <= rightInterval) {
            return it[p][l];
        }
        else {
            return it[p][r - (1 << p) + 1];
        }
    }

    size_t maxQueryIndex(size_t l, size_t r) const noexcept {
        size_t len = r - l + 1;
        unsigned p = log2[len];
        long leftInterval = dp[p][l];
        long rightInterval = dp[p][r - (1 << p) + 1];

        if (leftInterval >= rightInterval) {
            return it[p][l];
        }
        else {
            return it[p][r - (1 << p) + 1];
        }
    }

    long long sumQuery(size_t l, size_t r) const noexcept {
        long long result = 0;
        while (l <= r) {
            unsigned p = log2[r - l + 1];
            result += dp[p][l];
            l += (1 << p);
        }

        return result;
    }

    long long mulQuery(size_t l, size_t r) const noexcept {
        long long result = 1;
        while (l <= r) {
            unsigned p = log2[r - l + 1];
            result *= dp[p][l];
            l += (1 << p);
        }

        return result;
    }

    long query(size_t l, size_t r, std::function<long(long, long)> fn) const noexcept {
        size_t len = r - l + 1;
        unsigned p = log2[len];
        return fn(dp[p][l], dp[p][r - (1 << p) + 1]);
    }

    // Fast base 2 logarithm lookup table for 1 <= i <= n
    unsigned log2[n + 1] = { 0 };

    // The sparse table values.
    long dp[P + 1][n];

    // Index Table associated with the values in the sparse table.
    size_t it[P + 1][n];

    STOperation op;

    std::function<long(long, long)> sumFn = [](long a, long b) -> long { return a + b; };
    std::function<long(long, long)> minFn = [](long a, long b) -> long { return a < b ? a : b; };
    std::function<long(long, long)> maxFn = [](long a, long b) -> long { return a > b ? a : b; };
    std::function<long(long, long)> mulFn = [](long a, long b) -> long { return a * b; };
    std::function<long(long, long)> gcdFn = [](long a, long b) -> long { 
        long gcd = a;
        while (b) {
            gcd = b;
            b = a % b;
            a = gcd;
        }

        return (gcd ^ (gcd >> (sizeof(long) * 8 - 1))) - (gcd >> (sizeof(long) * 8 - 1));
    };

    void init(long (&v)[n]) noexcept {
        for (size_t i = 0; i < n; ++i) {
            dp[0][i] = v[i];
            it[0][i] = i;
        }

        for (size_t i = 2; i <= n; ++i) {
            log2[i] = log2[i / 2] + 1;
        }

        // Dynamic Programming: Build sparse table
        for (unsigned i = 1; i <= P; ++i) {
            for (size_t j = 0; j + (1 << i) <= n; ++j) {
                long leftInterval = dp[i - 1][j];
                long rightInterval = dp[i - 1][j + (1 << (i - 1))];

                switch (op) {
                    case STOperation::MIN: {
                        dp[i][j] = minFn(leftInterval, rightInterval);

                        // Propagate the index of the best value
                        if (leftInterval <= rightInterval) {
                            it[i][j] = it[i - 1][j];
                        }
                        else {
                            it[i][j] = it[i - 1][j + (1 << (i - 1))];
                        }

                        break;
                    }

                    case STOperation::MAX: {
                        dp[i][j] = maxFn(leftInterval, rightInterval);

                        // Propagate the index of the best value
                        if (leftInterval >= rightInterval) {
                            it[i][j] = it[i - 1][j];
                        }
                        else {
                            it[i][j] = it[i - 1][j + (1 << (i - 1))];
                        }

                        break;
                    }

                    case STOperation::SUM: {
                        dp[i][j] = sumFn(leftInterval, rightInterval);
                        break;
                    }

                    case STOperation::MUL: {
                        dp[i][j] = mulFn(leftInterval, rightInterval);
                        break;
                    }

                    case STOperation::GCD: {
                        dp[i][j] = gcdFn(leftInterval, rightInterval);
                        break;
                    }
                }
            }
        }
    }
};

/*int main(void) {
    long values[7] = {1, 2, -3, 2, 4, -1, 5};
    SparseTable<array_size(values)> st(values, STOperation::MAX);

    std::cout << st.query(3, 6) << std::endl;
    return 0;
}*/
