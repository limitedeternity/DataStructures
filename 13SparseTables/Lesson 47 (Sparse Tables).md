## Sparse Tables

Sparse tables are all about doing efficient **range queries** on **static arrays**. Range queries come in a variety of flavors, but the most common types are *min*, *max*, *sum* and *gcd* range queries.

For instance, we may want to know the maximum value between indexes [4, 12], the gcd between indexes [6, 9] or even the sum of the values between indexes [2, 11].

### Sparse Table Intuition

Every positive integer can be easily represented as a sum of powers of 2 given by its binary representation:

> $19 = 10011_2 = 2^4 + 2^1 + 2^0$

Similarly, any interval [l, r] can be broken down into smaller intervals of powers of 2:

> $[5, 17] = [5, 5+2^3)\ \cup\ [13, 13 + 2^2)\ \cup\ [17, 17 + 2^0) = [5, 13)\ \cup\ [13, 17)\ \cup\ [17, 18)$

Now, imagine if we could precompute the range query answer (i.e: min, max, sum, etc.) for all these intervals and combine them...

### Range combination function

A sparse table can help us answer all these queries efficiently. For **associative functions**, a sparse table can answer range queries in $O(log_2(n))$.

> A function f(x, y) is associative if:
> $\forall a, b, c. f(a, f(b, c)) = f(f(a, b), c)$.

Operations such as addition and multiplication are associative, but functions like subtraction and exponentiation are not. Here’s a counterexample:

> Let $f(a, b) = a - b$.

| $f(1, f(2, 3))$ | $f(f(1, 2), 3)$ |
| :-------------: | :-------------: |
|  $f(1, 2 - 3)$  |  $f(1 - 2, 3)$  |
|  $1 - (2 - 3)$  |  $(1 - 2) - 3$  |
|   $1 - (-1)$    |   $(-1) - 3$    |
|       $0$       |      $-4$       |

$\Rightarrow$ $f$ is not associative.

Logarithmic time range queries are really good, however we can do better. When the range query combination function is **“overlap friendly”**, then range queries on a sparse table can be satisfied in $O(1)$.

Being overlap friendly means a function yields the same result regardless of whether it’s combining ranges which overlap or those that don’t.

> We say a binary function $f(x, y)$ is overlap friendly if:
> $\forall a, b, c. f(f(a, b), f(b, c)) = f(a, f(b, c))$.

### Overlap friendly functions:

* $f(a, b) = 1 * b$
* $f(a, b) = min(a, b)$
* $f(a, b) = max(a, b)$
* $f(a, b) = \dfrac{a * b}{a}, a \ne 0$

* $f(a, b) = gcd(a, b)$

### Table construction

The idea behind a sparse table is to precompute the answer for all intervals of size $2^x$ to efficiently satisfy range queries between [l, r].

Let N be the size of the input values array, and let $2^P$ be the largest power of 2 that fits in the length of the values array.

> $P = floor(log_2(N)) = floor(log_2(13)) = 3$

* Begin by initializing a table with P+1 rows and N columns. Then, fill the first row with the input values.

Each cell (i, j) then will represent the answer for the range $[j, j+2^i)$ in the original array. For example, cell (2, 5) will represent the answer for the range [5, 9).

What do we do about cells representing ranges, that reach outside the bounds of the sparse table? We simply ignore these cells.

The way we’re going to build the table is by reusing the already computed range values of previous cells. Each cell represents the range $[j, j + 2^i)$ which always has even length. This range can be broken into two sub intervals (which we will have already computed).

More specifically, the range for cell (i, j) can be split into a left interval $[j, j + 2^{i - 1})$ and a right interval $[j+2^{i - 1}, j + 2^i)$ whose values would correspond to the cells $(i - 1, j)$ and $(i - 1, j+2^{i - 1})$ respectively.

* Finish filling the sparse table by combining values from the previous rows (dynamic programming).

### Range Queries

In our table we’ve already precomputed the answer for all intervals of length $2^x$. Let k be the largest power of two that fits in the length of the range between [l, r].

Knowing k, we can easily do a lookup in the table to find the answer in between the ranges $[l, l + k]$ (left interval) and $[r - k + 1, r]$ (right interval) to find the answer for [l, r]. The left and right intervals may overlap, but this doesn’t matter (given the overlap friendly property) so long as the entire range is covered.

### Associative Function Queries

Some functions such as multiplication and summation are associative, but *aren't* overlap friendly. A sparse table can still handle these types of queries, but in $O(log_2(n))$ rather than $O(1)$. The main issue with non-overlap friendly functions with our previous approach is that overlapping intervals would yield the wrong answer.

An alternative approach to performing a range query is to do a *cascading* query on the sparse table by breaking the range [l, r] into smaller ranges of size $2^x$ which **don’t overlap**.

For example, the range [2, 15] can be split into three intervals of lengths 8, 4 and 2:

> $[2, 15] = [2, 2+2^3)\ \cup\ [10, 10+2^2)\ \cup\ [14, 14 + 2^1) = [2, 10)\ \cup\ [10, 14)\ \cup\ [14, 16)$

