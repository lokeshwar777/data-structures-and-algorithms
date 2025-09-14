# Range Queries Notes

1. prefix sums / difference array
   - `f(r) - f(l - 1)` or `f(r + 1) - f(l)` (based on indexing - 0-based or 1-based)
2. sparse table
   - (index, power/exponent) -> `[index][k]` or `[index][k - 1]` (based on power `k`)
   - what is the aggregate of `2^k` elements starting at `index`
   - (preprocessing=`O(NlogN)`, query=`O(1)`)
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

- what is the best option to use? (static, dynamic) vs (invertible=sum/XOR..., non-invertible=min/max/gcd...)
  - coordinate compression `O(NlogN)`
  - static array (range queries)
    - invertible ops -> pref sum + diff array, precompute=`O(N)`, query=`O(1)`
    - non-invertible ops -> sp table, preprocessing=`O(NlogN)`, query=`O(1)`
    - offline -> mo's algo `O((N+Q)sqrtN)`
  - semi-dynamic array (updates + range queries)
    - point
      - invertible ops -> BIT/Fenw Tree (preprocessing=`O(NlogN)`, query=`O(logN)`)
      - non-invertible ops ->
        - (block) sqrt decomposition (preprocessing=`O(N)`, query=`O(sqrtN)`)
        - Seg Tree (preprocessing=`O(NlogN)`, query=`O(logN)`)
    - range
      - invertible ops -> 2-Fenw Tree (preprocessing=`O(NlogN)`, query=`O(logN)`)
      - non-invertible ops -> Lazy Seg Tree (preprocessing=`O(NlogN)`, query=`O(logN)`)
  - fully dynamic array (size changes)
  
  - Fully Dynamic Array (insert/delete/change size)
  - Invertible ops
    - Balanced BST (Treap, AVL) + Augmentation
  - Non-invertible ops
    - Dynamic Segment Tree
