## Computational complexity analysis

### Big-O Notation

> Big-O Notation gives an upper bound of the complexity in the **worst** case, helping to quantify performance as the input size becomes arbitrarily large.

* Constant Time: **O(1)**
* Logarithmic Time: **O(log(n))**
* Linear Time: **O(n)**
* “Linearithmic” Time: **O(nlog(n))**
* Quadric Time: **O(n^2)**
* Cubic Time: **O(n^3)**
* Exponential Time: **O(b^n)**, b > 1
* Factorial Time: **O(n!)**

### Big-O Properties

1. *O(n + c)* = **O(n)**

2. *O(cn)* = **O(n)**, c > 0

Let f be a function of n: *7log(n)^3 + 15n^2 + 2n^3 + 8*

3. *O(f(n))* = **O(n^3)**

### Big-O Examples

The following run in **O(1)**:

```python
i = 0
while i < 11:
  i += 1
```

The following run in **O(n)**:

```python
i = 0
while i < n:
  i += 3
```

> f(n) = n/3
>
> O(n/3) = **O(n)**

The following run in **O(n^2)**:

```python
for i in range(n):
  for j in range(i, n):
    ...
```

> f(n) = n + (n - 1) + (n - 2) + … + 3 + 2 + 1 = n (n+1) / 2 = n^2/2 + n/2
>
> O(n^2/2 + n/2) = **O(n^2)**

### Binary search (Big-O example)

```python
low = 0
high = n - 1
while low <= high:
  mid = (low + high) / 2
  if array[mid] == value:
    return mid
  elif array[mid] < value:
    low = mid + 1
  else:
    high = mid - 1
return -1
```

> f(n) = n/2 + n/4 + n/8 + … = log2(n)
>
> O(log2(n)) = **O(log(n))**

### Next example

```python
i = 0
while i < n:
  j = 0
  while j < 3 * n:
    j += 1
  j = 0
  while j < 2 * n:
    j += 1
  i += 1
```

>f(n) = n * (3n + 2n) = 5n^2
>
>O(5n^2) = **O(n^2)**

### Another example

```python
i = 0
while i < 3 * n:
  j = 10
  while j <= 50:
  	j += 1
  j = 0
  while j < n * n * n:
    j += 2
  i += 1
```

> f(n) = 3n * (40 + n^3/2) = 120n + 3n^4/2
>
> O(120n + 3n^4/2) = **O(n^4)**

### And more:

* Finding all subsets of a set – **O(2^n)**
* Finding all permutations of a string – **O(n!)**
* Merge sort – **O(nlog(n))**
* Iterating over all the cells in a matrix of size n by m – **O(nm)**

