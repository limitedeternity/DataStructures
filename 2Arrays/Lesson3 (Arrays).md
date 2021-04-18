## Static and dynamic arrays

### What’s a static array?

> A static array is a fixed length container containing n elements **indexable** from 0 to n - 1.

*Indexable* means that each slot in the array can be referenced with a number.

### Where’s a static array used?

1. Storing and accessing **sequential** data
2. Temporarily storing objects
3. Used by I/O routines as buffers
4. Lookup tables and inverse lookup tables
5. Can be used to return multiple values from a function
6. Used in dynamic programming to cache answers to subproblems

### Complexity

| Operation | Static array | Dynamic array |
| :-------: | :----------: | :-----------: |
|  Access   |     O(1)     |     O(1)      |
|  Search   |     O(n)     |     O(n)      |
|  Insert   |     n/a      |     O(n)      |
|  Append   |     n/a      |     O(1)      |
|  Delete   |     n/a      |     O(n)      |

> Resizes on append are considered being rare

### How can we implement a dynamic array?

One way is to use a static one.

1. Create a static array with an initial capacity
2. Add elements to the underlying static array, keeping track of the number of elements
3. If adding another element will exceed the capacity, then create a new static array twice the capacity and copy over elements from the original array.

> It’s important to grow the size of the dynamic array exponentially to keep the time complexity of insertions O(1) most of the time

