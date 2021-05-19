##  Priority Queues

### What is a Priority Queue?

> A priority queue is an Abstract Data Type (ADT) that operates similar to a normal queue except that *each element has a certain priority*. The priority of the elements in the priority queue determines the order in which elements are removed from the PQ.

**NOTE**: Priority queues only support *comparable data*, meaning, that the data inserted into the PQ must be able to be ordered in some way either from least to greatest or vice-versa. This is so that we are able to assign relative priorities to each element.

### What is a Heap?

> A heap is a *tree*-based DS that satisfies the *heap invariant* (also called heap property): If A is a parent node of B then A is ordered with respect to B for all nodes A, B in the heap.

### When and where is a PQ used?

* Used in certain implementations of Dijkstra’s Shortest Path algorithm.
* Anytime you need to dynamically fetch the “next best” or “next worst” element.
* Used in Huffman coding (which is often used for lossless data compression).
* Best-First Search (B-FS) algorithms such as A* use PQs to continuously grab the next most promising node.
* Used by Minimum Spanning Tree (MST) algorithms.

### Complexity: PQ with binary heap

|                  Operation                  | Complexity |
| :-----------------------------------------: | :--------: |
|                Construction                 |    O(n)    |
|                   Polling                   | O(log(n))  |
|                   Peeking                   |    O(1)    |
|                   Adding                    | O(log(n))  |
|               Naive Removing                |    O(n)    |
| Advanced Removing with help of Hash Table*  | O(log(n))  |
|               Naive Searching               |    O(n)    |
| Advanced Searching with help of Hash Table* |    O(1)    |

> \* Using a Hash Table to optimize these operations takes up linear space and also adds some overhead to the binary heap implementation.

### Turning Min PQ into Max PQ

**Problem**: Often the standard library of most programming languages only provide a Min PQ which sorts by smallest elements first, but sometimes we need a Max PQ.

> Since elements in a PQ are comparable they implement some sort of *comparable interface* which we can simply *negate* to achieve a Max heap.

> An alternative method for numbers is to negate the numbers as you insert them into the PQ and negate them again when they are taken out.

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

### Removing Elements From a Binary Heap

When we *poll* the PQ, the root node has to be removed. So, we swap the value of the root node with the **bottom-right** leaf and remove the latter. After that, we perform a so-called “Bubbling Down” operation.

**“Bubbling Down” operation**: We begin to swap values with a child node (with *least* value if the heap is Min Heap or with *greatest* value if Max Heap while leaning to the **left** node if children values are *equal*) until heap invariant is satisfied or the bottom is reached.

When we remove the *value* from the PQ, we first search the node with this value and then perform a “Bubbling Down” operation **ignoring** heap invariant satisfaction. When the bottom is reached, we swap values with the **rightmost** node and remove the latter.

### Removing Elements From a Binary Heap in O(log(n))

The inefficiency of the removal algorithm comes from the fact that we have to perform a linear search to find out where an element is indexed at. What if instead we did a lookup using a *Hash Table* to find out where a node is indexed at?

A *Hash Table* provides a constant time lookup and update for a mapping from a key (the node value) to value (the index).

**Dealing with the multiple value problem:**

Instead of one-to-one mapping we will use one-to-many. We can maintain a *Set* or *Tree Set* of indexes for which a particular node value (key) maps to.

