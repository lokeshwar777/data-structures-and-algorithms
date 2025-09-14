# Graph Notes

- Basic
  - component (general), connected component (undirected), SCC (directed)
  - undirected - maximal set where all nodes are mutually reachable
  - directed - maximal set where every node can reach every other by following edge directions

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

- hamiltonian (undirected)
  - path = visit all vertices/nodes exactly once
  - single component
  - starting point matters
  - NP-complete problem (backtracking)
  - cycle = hamiltonian path + start = end

- Minimum Spanning Tree (MST)
  - Prim's
    - start with a src, PQ stores (u, v), u is part of MST, v is outside
    - pick min-weight edge from PQ, if v is visited (ignore), else consider it
    - add neighbour edges of the vertex visited, return edges of MST
  - Kruskal's
    - sort edges in ascending order of weights
    - pick edges one by one and use DSU/union find to check for connectivity/cycles

- bridges / cut-edge (Tarjan's algorithm)
  - upon edge removal conn_components++
  - edge that is part of a cycle can't be a bridge
  - discovery time of one node should be less than low of the other in a cut edge
  - main idea (bridge): child node shouldn't indirectly reach parent (`disc[parent] < low[child]`)

- articulation point / cut-vertex (Tarjan's algorithm)
  - upon node removal conn_components+=x
  - am i the critical point and the only way for my children to reach the ancestors?
  - low time of any neighbouring node of a cut vertex should be
    less than or equal to discovery time of cut vertex
  - main idea (parent is AP): any child node shouldn't indirectly reach ancestor (`disc[parent] <= low[child]`)
  - edge case: 1 < DFS for root

- (SCC) strongly connected components
  - Kosaraju's algorithm - two pass
    1. record finish order / kind of topo sort order via DFS
    2. reverse edges
    3. collect nodes in an SCC via DFS
  - Tarjan's algorithm - single pass, uses low-link
    - different from bridges/APs, uses the concept of "in-stack"
    - ignore collected SCCs
    - back edge is only considered if the visited node is part of "in-stack"
    - low time is the smallest reachable discovery time of the nodes in the stack
    - main idea (SCC) : pop until (`disc[u] == low[u]`)
  - DSU
  - Gabow’s

- union find or DSU (Disjoint Set Union)
  - naive -> O(N)
  - optimal -> find (path compression) + union (size/rank) - O(⍺(N)) (amortized complexity)
    - find - path compression -> O(⍺(N))
    - union - size/rank -> O(logN)

- advanced algorithms
  - Borůvka’s - MST
  - Johnson’s - cycle finding, APSP in $O(VE logV)$
  - A* → Heuristic-guided shortest path
  - Hierholzer’s Algorithm → Eulerian path/circuit. $(O(E))$
  - Held–Karp → Hamiltonian cycle / TSP via DP. $(O(V² 2^V))$

- Flow / Matching
  - Goal: max flow, min cut, bipartite matching.
  - Ford–Fulkerson / Edmonds–Karp → Max flow (augmenting paths). $(O(VE²))$
  - Dinic’s Algorithm → Faster max flow with level graphs. $(O(E√V))$
  - Push-Relabel (Goldberg–Tarjan) → Max flow with preflow + push. $(O(V³))$
  - Hopcroft–Karp → Maximum bipartite matching. $(O(√V E))$
  - Edmonds’ Algorithm → Minimum arborescence (directed MST).
  - Chu–Liu/Edmonds → Minimum arborescence (directed MST).

## Additional Variations

- binary search + connected components

## Implementations

- have a look at submissions on CSES, GFG
