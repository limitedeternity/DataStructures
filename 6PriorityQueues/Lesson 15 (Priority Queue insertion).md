## Priority Queue insertion

### Priority Queue with Binary Heap

There are many types of heaps we could use to implement a priority queue including:

* Binary Heap
* Fibonacci Heap
* Binomial Heap
* Pairing Heap
* <…>

### Binary Heap Representation

> Binary Heap can be represented as an array. Array is usually being constructed level-by-level left-to-right.

Let *i* be the parent node index.
Then, left child index will be *2i + 1* and right child index will be *2i + 2*.

### Adding Elements to Binary Heap

We insert the node at the very bottom and perform a so-called “Bubbling Up” operation.

**“Bubbling Up” operation**: We begin to swap values with the parent node until heap invariant is satisfied or the top is reached.

> The structure of a binary heap used for a priority queue should always be in the form of a *complete tree* to maintain an insertion point.

