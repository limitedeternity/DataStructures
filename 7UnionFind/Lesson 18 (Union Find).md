## Union Find

### What is Union Find?

**Union Find** is a data structure that keeps track of elements which are split into one or more disjoint sets. It has two primary operations: *find* and *union*.

### When and where is a Union Find used?

* Kruskal’s minimum spanning tree algorithm
* Grid percolation
* Network connectivity
* Least common ancestor in trees
* Image processing

### Complexity

|     Operation      | Complexity  |
| :----------------: | :---------: |
|    Construction    |    O(n)     |
|       Union        | $\alpha(n)$ |
|        Find        | $\alpha(n)$ |
| Get component size | $\alpha(n)$ |
| Check if connected | $\alpha(n)$ |
|  Count components  |    O(1)     |

\* *$\alpha(n)$ – Amortized constant time*

### Union Find application: Kruskal’s Minimum Spanning Tree

Given a graph G = (V,E) we want to find a **MST** in the graph (it may not be unique).

A **MST** is a subset of the edges which connect all vertices in the graph with the minimal total edge cost.

1. Sort edges by ascending edge weight.
2. Walk through the sorted edges and look at the two nodes the edge belongs to; if the nodes are already unified we don’t include this edge, otherwise we include it and unify the nodes.

3. The algorithm terminates when every edge had been processed or all the vertices have been unified.

### Creating Union Find

1. Construct a **bijection** (a mapping) between your objects and integers in range [0, n).

**NOTE**: This step is not necessary in general, but it will allow us to construct an array-based union find.

2. Store union find information in an array. Each index has an associated object we can lookup through our mapping.

### Find Operation

To **find** which component a particular element belongs to, find the root of that component by following the parent nodes until a self-loop is reached (a node whose parent is itself).

### Union Operation

To **unify** two elements find which are the root nodes of each component and if the root nodes are different make one of the root nodes be the parent of the other.

### Remarks

In this data structure, we don’t “un-union” elements. In general, this would be very inefficient to do since we would have to update all the children of a node.

The number of components is equal to the number of roots remaining. Also, the number of root nodes never increases.

### Path Compression

> Union Operation side-effect, which is a key to that amortized constant time.

