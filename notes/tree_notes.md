# Tree Notes

- Overview
  - Height of the Tree = max(Depth of all nodes in the tree)
  - DP on trees, rerooting
  - dist/ancestors -> binary lifting + LCA (Lowest Common Ancestor)
  - queries
    - subtree -> ETT / euler tour (in-out) + seg tree
    - path -> HLD (Heavy Light Decomposition)
    - freq/color → DSU on Trees or Mo’s.
  - divide & conquer -> Centroid Decomposition
  - Link-Cut Trees
  - General Traversal - recusion/stack space ($O(N or logN)$ space)
  - Morris Traversal - tree threading ($O(1)$ space)
  - options/techniques
    - prefix aggregate
    - binary lifting
    - ETT
    - HLD
    - centroid decomposition

## Data Structures

- Basic

- Binary
- BST
- AVL
- Red-Black

## Algorithms

- Binary Lifting
  - LCA (preprocessing=`O(NlogN)`, query=`O(logN)`)
  - non-invertible path queries in a static tree
  - invetible path queries in a dynamic tree
  - partial sums -> push updates from bottom to top (preprocessing=`O(NlogN)`, update=`O(logN)`)
  - usage: monotonic stack/queue, sparse table, chains, kth ancestor, tree extension

- Euler Tour on Trees (ETT)
  - subtree rooted at `u` = interval `[in[u], out[u]]`
  - nested intervals -> ancestor/descendant
  - disjoint intervals -> diff branches (siblings/cousins)
  - good for subtree queries because we can use BIT/seg tree on contiguous ranges
  - paths are not contiguous
  1. (1N ETT) entry only Euler Tour
     - good for subtree queries, ancestor/subtree check
     - `in[u]` -> first & only time we visit a node `u`
     - subtree size = `out[u] - in[u] + 1`
     - flattening (tree -> array) (entry only)
       - `flat[N]` -> flattened array
       - each node apprears once in `flat[]`
       - store the props related to a node `u` at `in[u]`th index in `flat[]`, so now
       - `subtree_query(root=u)` becomes `range_query(from=in[u], to=out[u])` (subtree=contiguous block)
       - `update(node=u,newVal=x)` becomes `point_update(index=in[u])`
       - why this is not good for path queries?
         - prefix sum of a range from `root` to `u` in the `flat[]` doesn't represent
               only the path from `root` to `u`, instead it actually represents the nodes which were
               visited before `u`
         - this is the reason we need to either move to
               1. entry + exit (2N) (invertible ops)
               2. HLD (non-invertible ops)
  2. (2N ETT) entry + exit Euler Tour
     - good for invertible path queries, LCA
     - `in[u]` -> first visit time (entry), `out[u]` -> last visit time (exit)
     - subtree size = `(out[u] - in[u] + 1) / 2`
     - flattening (tree -> array) (entry + exit)
       - `flat[2N]` -> flattened array
       - each node apprears twice in `flat[]`

- Heavy-Light Decomposition (HLD)

- Centroid Decomposition

- what is the best option to use? (static, dynamic) vs (subtree, path) vs (invertible=sum/XOR..., non-invertible=min/max/gcd...)
  - Diameter
    - single pass -> height + max tracking
    - 2 pass -> 2 DFS/BFS trick + farthest node
  - Center
  - Centroid
  - static tree (queries only)
    - LCA
      - Binary Lifting (preprocessing=`O(NlogN)`, query=`O(logN)`)
      - 2N ETT (Euler Tour + RMQ) (preprocessing=`O(NlogN)`, query(sparse table)=`O(1)`)
    - subtree -> DFS+simple suffix aggregation (preprocessing=`O(N)`, query=`O(1)`)
    - path
      - invertible ops -> precompute=(DFS+simple prefix aggregate, `O(N)`), query=(pref+LCA, `O(logN)`)
      - non-invertible ops -> binary lifting (preprocessing=`O(NlogN)`, query=`O(logN)`)
  - dynamic tree (updates + queries)
    - subtree
      - invertible ops -> 1N ETT + BIT/Fenw Tree (preprocessing=`O(NlogN)`, query=`O(logN)`)
      - non-invertible ops -> 1N ETT + Seg Tree (preprocessing=`O(NlogN)`, query=`O(logN)`)
    - path
      - invertible ops -> 2N ETT + BIT/Fenw Tree
      - non-invertible ops -> HLD + Seg Tree
  - fully dynamic -> Link-Cut Tree / Balanced ETT

## Non-intuitive ideas

- `path(a,b)`=`path(root,a)`+`path(root,b)`-`2*path(root,LCA(a,b))`+`val[LCA(a,b)]`
- if no node values then consider edge weights above each node as node values
    & do not include LCA in the result & set root value to neutral
