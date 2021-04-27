## Priority Queue removal

### Removing Elements From a Binary Heap

When we *poll* the PQ, the root node has to be removed. So, we swap the value of the root node with the **bottom-right** leaf and remove the latter. After that, we perform a so-called “Bubbling Down” operation.

**“Bubbling Down” operation**: We begin to swap values with a child node (with *least* value if the heap is Min Heap or with *greatest* value if Max Heap while leaning to the **left** node if children values are *equal*) until heap invariant is satisfied or the bottom is reached.

When we remove the *value* from the PQ, we first search the node with this value (DFS if Min Heap, BFS if Max Heap or Hash Table, if available) and perform a “Bubbling Down” operation **ignoring** heap invariant satisfaction. When the bottom is reached, we swap values with the **rightmost** node and remove the latter.

### Removing Elements From a Binary Heap in O(log(n))

The inefficiency of the removal algorithm comes from the fact that we have to perform a linear search to find out where an element is indexed at. What if instead we did a lookup using a *Hash Table* to find out where a node is indexed at?

A *Hash Table* provides a constant time lookup and update for a mapping from a key (the node value) to value (the index).

**Dealing with the multiple value problem:**

Instead of one-to-one mapping we will use one-to-many. We can maintain a *Set* or *Tree Set* of indexes for which a particular node value (key) maps to.

