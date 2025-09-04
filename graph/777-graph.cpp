/* 777 */
#include "bits/stdc++.h"
using namespace std;

template <typename T>
using min_pq = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
    int u, v, w;
    Edge(int from, int to, int weight) : u(from), v(to), w(weight) {}
};

struct Node {
    int u, cst;
    Node(int node, int cost) : u(node), cst(cost) {}

    // max-heap
    bool operator<(const Node& other) const { return cst > other.cst; };

    // min-heap
    // bool operator<(const Node& other) const { return cst < other.cst; };
};

const int MAX_N = 2e6 + 5, INF = 1e9 + 5;  // 4e18
const int MAX_NN = 505;
int N, M, parent[MAX_N], dist[MAX_N], path_vis[MAX_N], indeg[MAX_N],
    colors[MAX_N], dp[MAX_NN][MAX_NN];
vector<pair<int, int>> g[MAX_N];  // adj list : g[u] is list of {v, w}
bitset<MAX_N> vis;
vector<int> cycle_path;

// ---------------- basic ----------------------

void add_edge(int u, int v, int w = 1, bool dir = false) {
    g[u].emplace_back(v, w);
    indeg[v]++;
    if (!dir) {
        g[v].emplace_back(u, w);
        indeg[u]++;
    }
}

void build_graph(const vector<Edge>& edges, bool dir = false) {
    for (int i = 1; i <= N; ++i) {
        g[i].clear();
        path_vis[i] = indeg[i] = 0;
        colors[i] = parent[i] = -1;
        dist[i] = INF;
    }
    vis.reset();
    for (auto& e : edges) {
        add_edge(e.u, e.v, e.w, dir);  // (from, to, weight, isDirected)
    }
}

// -------------- traversals -------------------

void dfs(int u, int p = -1) {
    vis[u] = path_vis[u] = 1;
    parent[u] = p;

    for (auto& [v, w] : g[u]) {
        if (v == p) continue;
        if (!vis[v]) {
            dfs(v, u);
        } else {
        }
    }
}

void bfs(int src) {
    queue<int> q;
    q.push(src);
    vis[src] = 1;
    parent[src] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& [v, w] : g[u]) {
            if (vis[v]) continue;
            vis[v] = 1;
            parent[v] = u;
            q.push(v);
        }
    }
}

// ----------------- utils ---------------------

int conn_cmpts() {
    int cnt = 0;

    for (int i = 1; i <= N; ++i) {
        if (vis[i]) continue;
        cnt++;
        dfs(i);
    }

    return cnt;
}

vector<int> get_path(int src, int dst) {
    vector<int> path;

    while (dst != -1 && (path.empty() || dst != src)) {
        path.push_back(dst);
        dst = parent[dst];
    }
    path.push_back(src);

    reverse(path.begin(), path.end());
    return path;
}

int neg_cycle_path(int u) {
    for (int i = 1; i <= N; ++i) {
        u = parent[u];
        if (u == -1) return 0;
    }
    // now u is part of cycle
    for (int x = u; cycle_path.empty() || x != u; x = parent[x])
        cycle_path.push_back(x);
    cycle_path.push_back(u);
    reverse(cycle_path.begin(), cycle_path.end());
    return 1;
}

bool neg_cycle(const vector<Edge>& edges) {
    for (Edge e : edges) {
        int u = e.u, v = e.v, w = e.w;
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            parent[v] = u;
            return neg_cycle_path(v);
        }
    }
    return 0;
}

// ---------------- shortest -------------------
// can also be implemented using DFS but not efficient for larger graphs

// unweighted
int shortest_BFS(int src, int dst) {
    queue<int> q;
    q.push(src);
    vis[src] = 1;
    parent[src] = -1;

    for (int lvl = 1; !q.empty(); lvl++) {
        for (int sz = q.size(); sz; --sz) {
            int u = q.front();
            q.pop();
            if (u == dst) return lvl;
            for (auto& [v, w] : g[u]) {
                if (vis[v]) continue;
                vis[v] = 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    return -1;
}

// 0/1 weights
int shortest_01BFS(int src, int dst) {
    deque<pair<int, int>> q;
    q.push_back({src, 0});
    vis[src] = 1;
    parent[src] = -1;

    while (!q.empty()) {
        auto [u, cst] = q.front();
        q.pop_front();
        if (u == dst) return cst;
        for (auto& [v, w] : g[u]) {
            if (vis[v]) continue;
            vis[v] = 1;
            parent[v] = u;
            if (w) {
                q.push_back({v, cst + 1});
            } else {
                q.push_front({v, cst});
            }
        }
    }

    return -1;
}

// Directed Acyclic Graph - SSSP
vector<int> shortest_DAG(int src = 1) {
    dist[src] = 0;
    vector<int> topo_sort = topo_sort_BFS();
    for (int u : topo_sort) {
        for (auto& [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) dist[v] = dist[u] + w;
        }
    }
    for (int i = 1; i <= N; ++i) {
        if (dist[i] == INF) dist[i] = -1;
    }
    return vector<int>(dist + 1, dist + N + 1);
}

// O(V ^ 2) for dense graphs, array-based - SSSP
vector<int> dijkstra_dense(int src = 1) {
    dist[src] = 0;

    for (int i = 1; i <= N; ++i) {
        int u = -1;
        for (int j = 1; j <= N; ++j) {
            if (vis[j] || dist[j] == INF) continue;
            if (u == -1 || dist[u] > dist[j]) u = j;
        }
        if (u == -1) break;
        vis[u] = 1;
        for (auto& [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    return vector<int>(dist + 1, dist + N + 1);
}

// O(E * log V) for sparse graphs using min-heap/PQ (uses dist[] instead of
// vis[])
int dijkstra(int src, int dst) {
    // max-heap
    // priority_queue<Node> pq;  // uses operator< inside struct

    // min-heap
    auto cmp = [](const Node& a, const Node& b) { return a.cst > b.cst; };
    priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);

    pq.push(Node(src, 0));
    dist[src] = 0;
    parent[src] = -1;

    while (!pq.empty()) {
        auto node = pq.top();
        pq.pop();
        int u = node.u, cst = node.cst;
        if (dist[u] < cst) continue;  // skip stale entries during relaxation
        if (u == dst) return cst;
        for (auto& [v, w] : g[u]) {
            int ncst = cst + w;
            if (ncst < dist[v]) {
                dist[v] = ncst;
                parent[v] = u;
                pq.push(Node(v, ncst));
            }
        }
    }

    return INF;
}

// Bellman–Ford algorithm -> O(V * E), -ve weights + no cycles
// V - 1 relaxations - shortest path (simple) can have atmost V - 1 edges
// change in distance in the Vth relaxation detects negative cycle
// dependent on source node
int bellman_ford(const vector<Edge>& edges, int src = -1) {
    for (int i = 1; i <= N; ++i) {
        dist[i] = (src == -1 || i == src) ? 0 : INF;
        parent[i] = -1;
    }
    for (int i = 1; i <= N - 1; ++i) {
        for (Edge e : edges) {
            int u = e.u, v = e.v, w = e.w;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }
    return neg_cycle(edges);  // actual negative cycle check
}

// Floyd–Warshall algorithm -> O(V^3) - APSP
// dp[i][j][k] - shortest path (i -> j) using only {0,..,k} as intermediaries
// compressed into one dist_mat as we update the previous state on each
// iteration
void floyd_warshall() {
    for (int k = 1; k <= N; ++k) {          // intermediate
        for (int i = 1; i <= N; ++i) {      // src
            for (int j = 1; j <= N; ++j) {  // dest
                if (dp[i][k] == INF || dp[k][j] == INF) continue;
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
            }
        }
    }
}

// Shortest Path Faster Algorithm - optimised Bellman-Ford algo
int SPFA() {
    return -1;
}

// ---------------- cycles ---------------------

void topo_sort_DFS(int u, vector<int>& topo_sort) {
    if (vis[u]) return;

    vis[u] = 1;
    topo_sort.push_back(u);

    for (auto& [v, w] : g[u]) {
        if (--indeg[v] == 0) topo_sort_DFS(v, topo_sort);
    }
}

vector<int> topo_sort_BFS() {
    vector<int> topo_sort;

    queue<int> q;
    for (int u = 1; u <= N; ++u) {
        if (indeg[u] == 0) q.push(u);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_sort.push_back(u);
        for (auto& [v, w] : g[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }

    return topo_sort;
}

// cycle detection: DFS + undirected
bool has_cycle_DFS_undir(int u, int p = -1) {
    vis[u] = 1;
    parent[u] = p;

    for (auto& [v, w] : g[u]) {
        if (v == p) continue;
        if (vis[v]) {
            cycle_path = get_path(v, u);
            cycle_path.push_back(v);
        }
        if (vis[v] || has_cycle_DFS_undir(v, u)) return 1;
    }

    return 0;
}

// cycle detection: BFS + undirected
bool has_cycle_BFS_undir(int src = 1) {
    queue<pair<int, int>> q;
    q.push({src, -1});
    vis[src] = 1;
    parent[src] = -1;
    while (!q.empty()) {
        auto& [u, p] = q.front();
        q.pop();
        for (auto& [v, w] : g[u]) {
            if (v == p) continue;
            if (vis[v]) {
                vector<int> p1 = get_path(src, u);
                vector<int> p2 = get_path(src, v);
                int i = 0;
                while (p1[i + 1] == p2[i + 1]) ++i;
                cycle_path.insert(cycle_path.end(), p1.begin() + i, p1.end());
                cycle_path.insert(cycle_path.end(), p2.rbegin(), p2.rend() - i);
                return 1;
            }
            parent[v] = u;
            vis[v] = 1;
            q.push({v, u});
        }
    }
    return 0;
}

// cycle detection: DFS + directed
bool has_cycle_DFS_dir(int u, int p = -1) {
    vis[u] = path_vis[u] = 1;
    parent[u] = p;

    for (auto& [v, w] : g[u]) {
        if (path_vis[v]) {
            cycle_path = get_path(v, u);
            cycle_path.push_back(v);
            return 1;
        }
        if (!vis[v] && has_cycle_DFS_dir(v, u)) return 1;
    }

    path_vis[u] = 0;  // backtrack
    return 0;
}

// cycle detection: BFS + directed (no path)
bool has_cycle_BFS_dir() {
    vector<int> topo_sort = topo_sort_BFS();
    return (int) topo_sort.size() != N;
}

// BFS lacks a recursion stack/state history, so it can’t trace back edges to
// rebuild the actual cycle path in directed graphs.

// ----------------- coloring/bipartite ---------------------

// colors should be initially not set i.e -1, returns is bipartite
bool coloring_DFS(int u, int p = -1, int cur = 0) {
    if (colors[u] != -1) return colors[u] == cur;
    colors[u] = cur;
    for (auto& [v, w] : g[u]) {
        if (v == p) continue;
        if (!coloring_DFS(v, u, !cur)) return 0;
    }
    return 1;
}

bool coloring_BFS(int src, int start = 0) {
    return 1;
}

// ----------------- algos ---------------------

// kahn's algo = Topological Sort via BFS on DAGs
// kosaraju's / tarjan's algo
// prim's algo
// kruskal's algo
// johnson’s algo

// ----------------- TODOs ----------------------

// Bridges / Articulation
vector<Edge> get_bridges() {
    vector<Edge> bridges;
    // TODO
    return bridges;
}
// MSTs
// dijkstra using set
// optimised dijkstra from latest research paper 2025
// negative cycle sum detection
// complexities for all

// ----------------- code ----------------------

void solve() {
    // ---INPUT---
    cin >> N >> M;
    vector<Edge> edges;
    for (int u, v, i = 0; i < M; ++i) {
        cin >> u >> v;
        // u++; v++; // if input is 0-based
        edges.emplace_back(u, v, 1);

        // weighted
        // int w;
        // cin >> u >> v >> w;
        // u++; v++; // if input is 0-based
        // edges.emplace_back(u, v, w);
    }

    // ---BUILD---
    build_graph(edges, false);
    // 1 is the root

    // ---DFS---
    // int cnt = 0;
    // for (int i = 1; i <= N; ++i) {
    //     if (vis[i]) continue;
    //     cnt++;
    //     dfs(i);
    // }

    // ---BFS---
    // bfs(1);

    // ---CONNECTED COMPONENTS---
    // int cmpts = conn_cmpts();

    // ---SHORTEST PATH---
    // int len = shortest_BFS(1, N);
    // int len = shortest_01BFS(1, N);
    // int len = dijkstra(1, N);
    // vector<int> dists = shortest_DAG();
    // vector<int> dists = dijkstra_dense(1);
    // vector<int> path = get_path(1, N);
    // int has_neg_cycle = bellman_ford(edges, 1);
    // for (int i = 1; i <= N; ++i) {
    //     for (int j = 1; j <= N; ++j) dp[i][j] = INF;
    // }
    // floyd_warshall();

    // ---COLORING---
    // for (int i = 1; i <= N; ++i) {
    //     if (colors[i] != -1) continue;
    //     int possible = coloring_DFS(i);
    //     if (!possible) {
    //     }
    // }

    // ---CYCLES---
    // vector<int> topo_sort = topo_sort_BFS();
    // vector<int> topo_sort;
    // for (int i = 1; i <= N; ++i) {
    //     if (indeg[i] == 0) topo_sort_DFS(i, topo_sort);
    // }
    // bool has_cycle = (int) topo_sort.size() == N;

    // bool has_cycle = 0;
    // for (int i = 1; i <= N && !has_cycle; ++i) {
    //     if (vis[i]) continue;
    //     has_cycle |= has_cycle_DFS_undir(i);
    //     has_cycle |= has_cycle_BFS_undir(i);
    //     has_cycle |= has_cycle_DFS_dir(i);
    // }
    // bool has_cycle = has_cycle_BFS_dir();
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T = 1;
    // cin >> T;
    while (T--) solve();
}

/*
WHY to use dist[]?
 - avoid pushing the same node multiple times (extra pushes)
 - store shortest distances for all nodes

topological sort
 - linear ordering of vertices/nodes such that u comes before v for every
directed edge u->v
 - only for DAG
 - multiple orderings are possible

checkpoints
- iterate over all components?
- build properly - directed / undirected?
- proper usage of long long (instead of int)?
- INF set to 4e18 (instead of 1e9)?
- proper init of arrs like parent, dist, ...?
- 0-based/1-based complete conversion (choose only one among two)?
- reset graph / data structures after each test case?
- multiple / duplicate edges between two nodes?

non-intuitive ideas
- negate weights -> maximise becomes minimise or minimise becomes maximise
- invert direction of the edges (for directed) -> path from 1->N becomes N->1
- spread bad nodes completely (mark nodes via BFS/DFS)
*/