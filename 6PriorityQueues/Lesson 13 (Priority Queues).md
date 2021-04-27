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

