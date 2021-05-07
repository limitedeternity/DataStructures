## Turning Min PQ into Max PQ

**Problem**: Often the standard library of most programming languages only provide a Min PQ which sorts by smallest elements first, but sometimes we need a Max PQ.

> Since elements in a PQ are comparable they implement some sort of *comparable interface* which we can simply *negate* to achieve a Max heap.

> An alternative method for numbers is to negate the numbers as you insert them into the PQ and negate them again when they are taken out.

