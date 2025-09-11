# Range Queries Notes

1. prefix sums / difference array
   - `f(r) - f(l - 1)` or `f(r + 1) - f(l)` (based on indexing - 0-based or 1-based)
2. sparse table
   - (index, power/exponent) -> `[index][k]` or `[index][k - 1]` (based on power `k`)
   - what is the aggregate of `2^k` elements starting at `index`
3. BIT / fenwick tree -> point update, range query
   - 1-based tree so `TREE_SIZE = N + 1`
   - update(index, value), query(end) - query(start - 1)
4. 2 fenwick trees
   - difference arrays (not simple values)
   - update(start index, end index, delta/difference) (do not forget to update in array also)
   - query(start, end - 1)
5. Segment Tree
6. Lazy propogation
7. Square Root Decomposition
8. MO's algorithm
