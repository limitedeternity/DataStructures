#pragma once

#include <iostream>

template <size_t n>
class UnionFind {
    static_assert(n > 0, "Size must be natural");

public:
    UnionFind() noexcept {
        for (size_t i = 0; i < n; ++i) {
            id[i] = i; // self-root
            sz[i] = 1;
        }
    }

    virtual ~UnionFind() = default;

    // Find which component/set 'p' belongs to, takes amortized constant time.
    size_t find(size_t p) noexcept {

        // Find the root of the component/set
        size_t root = p;
        while (root != id[root]) root = id[root];

        // Compress the path leading back to the root.
        // Doing this operation is called "path compression"
        // and is what gives us amortized time complexity.
        while (id[p] != root) {
            size_t next = id[p];
            id[p] = root;
            p = next;
        }

        return root;
    }

    // Return whether or not the elements 'p' and
    // 'q' are in the same components/set.
    bool connected(size_t p, size_t q) noexcept {
        return find(p) == find(q);
    }

    // Return the size of the component/set 'p' belongs to
    size_t componentSize(size_t p) noexcept {
        return sz[find(p)];
    }

    // Returns the number of remaining components/sets
    size_t components() const noexcept {
        return numComponents;
    }

    // Unify the components/sets containing elements 'p' and 'q'
    void unify(size_t p, size_t q) noexcept {
        size_t root1 = find(p);
        size_t root2 = find(q);

        // These elements are already in the same group
        if (root1 == root2) return;

        // Merge smaller component/set into the larger one.
        if (sz[root1] < sz[root2]) {
            sz[root2] += sz[root1];
            id[root1] = root2;
        } 
        else {
            sz[root1] += sz[root2];
            id[root2] = root1;
        }

        --numComponents;
    }

private:

    // The number of components in the union find
    size_t numComponents = n;

    // Used to track the size of each of the component
    size_t sz[n];

    // id[i] points to the parent of i, if id[i] = i then i is a root node
    size_t id[n];
};

/*int main(void) {
    UnionFind<10> uf;
    uf.unify(6, 9);

    std::cout << uf.find(9) << std::endl;
    return 0;
}*/
