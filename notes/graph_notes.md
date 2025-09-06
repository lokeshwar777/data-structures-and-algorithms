# Graph Notes

- Tree
  - Height of the Tree = max(Depth of all nodes in the tree)
  - DP on trees, rerooting
  - dist/ancestors -> binary lifting + LCA (Lowest Common Ancestor)
  - queries
    - subtree -> euler tour (in-out) + seg tree
    - path -> HLD (Heavy Light Decomposition)
    - freq/color → DSU on Trees or Mo’s.
  - Special problems → Centroid Decomposition, Tree DP, Diameter.
  - Dynamic trees → Link-Cut Trees, ETT.
  - DSU on trees

- Topological sort
  - linear ordering of vertices for a DAG such that for
    every directed edge from u to v, u comes before v
  - why DAG only instead of undirected or cycles?
  - multiple orderings

- len(topo sort using kahn's algo) != vertices -> cycle

- bi-partite
  - divide vertices into 2 sets such that no edge between 2 vertices from the same set
  - NO -> odd length cycle
  - YES -> even length cycle

- shortest path problems
  1. Single-Source Shortest Path (SSSP) - 1 src → all nodes
     - BFS (unweighted / 0-1 weights → 0-1 BFS with deque)
     - Topo Sort (DAG) → Relax edges in topo order (O(V+E))
     - dijkstra != BFS + PQ (non-negative weights)
       - $O(V ^ 2)$ -> Queue, dense graph
       - $O(E logV)$ -> PQ/Set, sparse graph ($O(V ^ 2 logV)$ in the worst case)
       - doesn't work on -ve weights
     - bellman ford (negative weights allowed)
       - directed graph with -ve weights -> $O(E * V)$
       - run V-1 relaxations → if any change in distance → negative cycle → IMPOSSIBLE
     - SPFA (Shortest Path Faster Algorithm)
  2. Multi-Source Shortest Path (MSSP) - multi-srcs → all nodes
     - Multi-source BFS (push all sources in queue initially)
     - Run Dijkstra with multiple sources (init PQ with all sources (dist=0))
     - floyd warshall
  3. All-Pairs Shortest Path (APSP)
     - Floyd–Warshall $(O(N ^ 3))$
       - independent of all edges
       - gradually include intermediary nodes
     - Repeated Dijkstra $(O(V * (E log V)))$
     - bellman ford $(O(V ^ 2 * E))$
     - Johnson’s Algorithm (good for sparse graphs with negative edges, no cycles).
  4. Single-Source Single-Destination Shortest Path
     - Dijkstra/BFS + early exit
  5. K-Shortest Paths
  6. Constrained Shortest Path
     - DP on graphs / modified Dijkstra
  7. Dynamic Shortest Path
  - IMPOSSIBLE -> (-ve wts undir) or (-ve cyl dir)

- euler path (undirected)
  - visit all edges exactly once
  - starting point matters
  - odd degree -> exactly 0/2 vertices
  - graph is connected (isolated vertices are ignored)

- euler circuit (undirected)
  - euler path + start = end
  - starting point can be any in the circuit if any
  - even degree -> all vertices
  - graph is connected (isolated vertices are ignored)

- Minimum Spanning Tree (MST)
  - Prim's
    - start with a src, PQ stores (u, v), u is part of MST, v is outside
    - pick min-weight edge from PQ, if v is visited (ignore), else consider it
    - add neighbour edges of the vertex visited, return edges of MST
  - Kruskal's

- bridges
  - critical connections
  - articulation point

- cycles
  - Johnson’s algorithm - cycle enumeration

- union find or DSU (Disjoint Set Union)
  - naive -> O(N)
  - optimal -> find (path compression) + union (size/rank) - O(⍺(N)) (amortized complexity)
    - find - path compression -> O(⍺(N))
    - union - size/rank -> O(logN)

## Additional Variations

- binary search + connected components

## Implementations

- have a look at submissions on CSES, GFG
