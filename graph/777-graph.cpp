/* 777 */
#include "bits/stdc++.h"
using namespace std;

template <typename T>
using min_pq = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
    int u, v, w;
    Edge(int from, int to, int weight) : u(from), v(to), w(weight) {}

    // max-heap (default, uses operator<)
    bool operator<(const Edge& other) const { return w < other.w; };

    // min-heap (greater<Edge>, uses operator>)
    bool operator>(const Edge& other) const { return w > other.w; };
};

struct Node {
    int u, cst;
    Node(int node, int cost) : u(node), cst(cost) {}

    // max-heap (default, uses operator<)
    bool operator<(const Node& other) const { return cst < other.cst; };

    // min-heap (greater<Node>, uses operator>)
    bool operator>(const Node& other) const { return cst > other.cst; };
};

const int MAX_N = 2e6 + 5, INF = 1e9 + 5;  // 4e18
const int MAX_NN = 505;
int N, M, parent[MAX_N], indeg[MAX_N];
int dist[MAX_N], dp[MAX_NN][MAX_NN];
int path_vis[MAX_N], colors[MAX_N];
int disc[MAX_N], low[MAX_N];
vector<pair<int, int>> g[MAX_N];  // adj list : g[u] is list of {v, w}
bitset<MAX_N> vis;
vector<int> cycle_path;

struct DSU {
    vector<int> par, sz;

    DSU(int n) {
        sz.resize(n + 1, 1);
        par.resize(n + 1);
        for (int i = 1; i <= n; ++i) par[i] = i;
    }

    int find(int u) { return par[u] = (u == par[u]) ? u : find(par[u]); }

    void union_sz(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (sz[a] > sz[b]) swap(a, b);
        par[a] = b;
        sz[b] += sz[a];
    }

    bool is_conn(int a, int b) { return find(a) == find(b); }
};

// ---------------- build ----------------------

void add_edge(int u, int v, int w = 1, bool dir = false) {
    g[u].emplace_back(v, w);
    indeg[v]++;
    if (!dir) {
        g[v].emplace_back(u, w);
        indeg[u]++;
    }
}

void build_graph(const vector<Edge>& edges, bool dir = false) {
    for (auto& e : edges) {
        add_edge(e.u, e.v, e.w, dir);  // (from, to, weight, isDirected)
    }
}

void reset_graph(int n) {
    for (int i = 1; i <= N; ++i) {
        g[i].clear();
        path_vis[i] = indeg[i] = 0;
        colors[i] = parent[i] = disc[i] = -1;
        dist[i] = low[i] = INF;
    }
    vis.reset();
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
    // ensure u is part of cycle
    for (int i = 1; i <= N; ++i) {
        u = parent[u];
        if (u == -1) return 0;
    }
    for (int x = u; cycle_path.empty() || x != u; x = parent[x]) {
        cycle_path.push_back(x);
    }
    cycle_path.push_back(u);
    reverse(cycle_path.begin(), cycle_path.end());
    return 1;
}

// are all non-zero degree nodes in a single component (undirected graph)
bool is_euler_conn() {
    bool done = 0;
    for (int i = 1; i <= N; ++i) {
        if (!indeg[i]) continue;
        if (!vis[i] && done) return 0;
        if (!vis[i]) dfs(i);
        done = 1;
    }
    return 1;
}

// reverse edges
vector<Edge> reverse_edges(const vector<Edge>& org_edges) {
    vector<Edge> rev_edges;
    for (auto e : org_edges) rev_edges.push_back(Edge(e.v, e.u, e.w));
    return rev_edges;
}

// marking directed graphs using DFS
void dir_dfs(int u, vector<int>& cmpt) {
    if (vis[u]) return;
    vis[u] = 1;
    cmpt.push_back(u);
    for (auto& [v, w] : g[u]) dir_dfs(v, cmpt);
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
    // priority_queue<Node> pq;  // uses operator<

    // min-heap
    min_pq<Node> pq;  // uses operator>

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

// TODO: dijkstra using set

// TODO: optimised dijkstra from latest research paper 2025

// Bellman–Ford algorithm -> O(V * E), -ve weights + no cycles
// V - 1 relaxations - shortest path (simple) can have atmost V - 1 edges
// change in distance in the Vth relaxation detects negative cycle
// returns whether the graph has a negative cycle starting at source=src
int bellman_ford(const vector<Edge>& edges, int src = -1) {
    for (int i = 1; i <= N; ++i) {
        dist[i] = (src == -1 || i == src) ? 0 : INF;
        parent[i] = -1;
    }
    for (int i = 1; i <= N; ++i) {
        for (Edge e : edges) {
            int u = e.u, v = e.v, w = e.w;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                parent[v] = u;
                if (i == N) return neg_cycle_path(v);
                dist[v] = dist[u] + w;
            }
        }
    }
    return 0;
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

// ---------------- cycles, paths & circuits---------------------

// record finish order / kind of topo sort order
void get_finish_order(int u, vector<int>& topo_order) {
    if (vis[u]) return;
    vis[u] = 1;
    for (auto& [v, w] : g[u]) get_finish_order(v, topo_order);
    topo_order.push_back(u);
}

void topo_sort_DFS(int u, vector<int>& topo_sort) {
    if (vis[u]) return;
    vis[u] = 1;
    for (auto& [v, w] : g[u]) topo_sort_DFS(v, topo_sort);
    topo_sort.push_back(u);
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

// kahn's algo = Topological Sort via BFS on DAGs
// cycle detection: BFS + directed (no path)
bool has_cycle_BFS_dir() {
    vector<int> topo_sort = topo_sort_BFS();
    return (int) topo_sort.size() != N;
}

// BFS lacks a recursion stack/state history, so it can’t trace back edges to
// rebuild the actual cycle path in directed graphs.

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

// TODO
bool coloring_BFS(int src, int start = 0) {
    return 1;
}

// euler path - 0/2 odd degree vertices
bool has_euler_path() {
    int odd_cnt = 0;
    for (int i = 1; i <= N; ++i) odd_cnt += indeg[i] & 1;
    return (!odd_cnt || odd_cnt == 2);  // + connectivity check
}

// euler circuit - all even degree vertices
bool has_euler_circuit() {
    for (int i = 1; i <= N; ++i) {
        if (indeg[i] & 1) return 0;
    }
    return 1;  // + connectivity check
}

// hamiltonian path helper DFS
bool ham_path_dfs(int u, int cnt, vector<int>& path) {
    if (vis[u]) return 0;  // pruning
    vis[u] = 1;
    path.push_back(u);
    if (cnt == N) return 1;  // base case (path found)

    for (auto& [v, w] : g[u]) {  // transitions
        if (ham_path_dfs(v, cnt + 1, path)) return 1;
    }

    // backtrack
    vis[u] = 0;
    path.pop_back();
    return 0;
}

// hamiltonian path (backtracking)
bool has_ham_path() {
    int vis_cnt;
    for (int i = 1; i <= N; ++i) {
        vis.reset();
        vector<int> path;
        if (ham_path_dfs(i, 1, path)) return 1;
    }
    return 0;
}

// hamiltonian cycle helper DFS (from a start `st`)
bool ham_cyl_dfs(int u, int cnt, const int st, vector<int>& path) {
    if (vis[u]) return 0;  // pruning
    vis[u] = 1;
    path.push_back(u);

    for (auto& [v, w] : g[u]) {  // transitions
        bool cyl_found = cnt == N && v == st;
        if (cyl_found || ham_cyl_dfs(v, cnt + 1, st, path)) return 1;
    }

    // backtrack
    vis[u] = 0;
    path.pop_back();
    return 0;
}

// hamiltonian cycle (backtracking) (untested)
bool has_ham_cylce() {
    int vis_cnt;
    for (int i = 1; i <= N; ++i) {
        vis.reset();
        vector<int> path;
        if (ham_cyl_dfs(i, 1, i, path)) return 1;
    }
    return 0;
}

// TODO : euler path printing (Hierholzer’s algo)
vector<int> get_euler_path() {
    vector<int> euler_path;
    return euler_path;
}

// TODO : euler circuit printing (Hierholzer’s algo)
vector<int> get_euler_circuit() {
    vector<int> euler_circuit;
    return euler_circuit;
}

// ----------------- algos ---------------------

// prim's algo
// PQ stores edges (u,v): u inside MST, v outside
// Add edge if v not visited
pair<int, vector<Edge>> prim_mst(int src = 1) {
    int tot_cst = 0;
    vector<Edge> mst_edges;
    min_pq<Edge> pq;
    vis[src] = 1;
    for (auto& [v, w] : g[src]) pq.emplace(src, v, w);
    while (!pq.empty()) {
        auto e = pq.top();
        pq.pop();
        // u is already part of MST
        int u = e.u, v = e.v, w = e.w;
        if (vis[v]) continue;
        vis[v] = 1;  // v also becomes part of MST
        tot_cst += w;
        mst_edges.push_back(e);
        for (auto& [vv, ww] : g[v]) {
            if (!vis[vv]) pq.emplace(v, vv, ww);
        }
    }
    if ((int) mst_edges.size() != N - 1) return make_pair(0, vector<Edge>());
    return make_pair(tot_cst, mst_edges);
}

// kruskal's algo
pair<int, vector<Edge>> kruskal_mst(vector<Edge> edges) {
    int tot_cst = 0;
    vector<Edge> mst_edges;
    DSU dsu(N);
    sort(edges.begin(), edges.end());
    for (Edge e : edges) {
        if (dsu.is_conn(e.u, e.v)) continue;
        tot_cst += e.w;
        mst_edges.push_back(e);
        dsu.union_sz(e.u, e.v);
    }
    if ((int) mst_edges.size() != N - 1) return make_pair(0, vector<Edge>());
    return make_pair(tot_cst, mst_edges);
}

// Bridges (tarjan's algo)
// u=node, p=parent, t=timer, bs=bridges
void tarjan_bridges(int u, int p, int& t, vector<Edge>& bs) {
    disc[u] = low[u] = t++;
    for (auto& [v, w] : g[u]) {
        if (v == p) continue;
        if (disc[v] == -1) {
            tarjan_bridges(v, u, t, bs);
            low[u] = min(low[u], low[v]);
            // neighbour `v` shouldn't indirectly reach parent `u` or its
            // ancestor
            if (disc[u] < low[v]) bs.push_back(Edge(u, v, w));
        } else {
            // explicit back edge handling, works without this but safe
            low[u] = min(low[u], disc[v]);
        }
    }
}

// Articulation Points (tarjan's algo)
void tarjan_aps(int u, int p, int& t, vector<int>& cut_pts) {
    disc[u] = low[u] = t++;
    int cnt = 0, ap = 0;  // cnt=no. of disjoint children
    for (auto& [v, w] : g[u]) {
        if (v == p) continue;
        if (disc[v] == -1) {
            cnt++;
            tarjan_aps(v, u, t, cut_pts);
            low[u] = min(low[u], low[v]);
            // any neighbour `v` can't indirectly reach ancestor of parent `u`
            ap |= disc[u] <= low[v];
        } else {
            // explicit back edge handling (REQUIRED!!!)
            low[u] = min(low[u], disc[v]);
        }
    }
    if ((p == -1 && cnt > 1) || (p != -1 && ap)) cut_pts.push_back(u);
}

// TODO: articulation points + bridges using a single timer function ?

// (SCC) Strongly Connected Components (kosaraju's algo) (2 pass)
vector<vector<int>> kosaraju_scc(const vector<Edge>& edges) {
    // record finish order / kind of topo sort order via DFS
    vector<int> topo_order;
    for (int i = 1; i <= N; ++i) get_finish_order(i, topo_order);
    reverse(topo_order.begin(), topo_order.end());

    // reverse edges
    vector<Edge> rev_edges = reverse_edges(edges);
    build_graph(rev_edges, true);

    // count SCC's traversing in topo order
    vector<vector<int>> scc;
    for (int x : topo_order) {
        if (vis[x]) continue;
        vector<int> cur_cmpt;
        dir_dfs(x, cur_cmpt);
        scc.push_back(cur_cmpt);
    }
    // for (auto& vec : scc) sort(vec.begin(), vec.end());
    // sort(scc.begin(), scc.end());
    return scc;
}

// SCC using tarjan's helper DFS
void tarjan_dfs(int u, int& t, vector<int>& in_stack,
                vector<vector<int>>& scc) {
    disc[u] = low[u] = t++;
    path_vis[u] = 1;
    in_stack.push_back(u);
    for (auto [v, w] : g[u]) {
        if (disc[v] == -1) {
            tarjan_dfs(v, t, in_stack, scc);
            low[u] = min(low[u], low[v]);
        } else if (path_vis[v]) {  // back edge to the node in stack
            low[u] = min(low[u], disc[v]);
        }
    }
    if (disc[u] == low[u]) {
        vector<int> cmpt;
        int x;
        do {
            x = in_stack.back();
            in_stack.pop_back();
            cmpt.push_back(x);
            path_vis[x] = 0;
        } while (x != u);
        scc.push_back(cmpt);
    }
}

// (SCC) Strongly Connected Components (tarjan's algo) (1 pass)
vector<vector<int>> tarjan_scc() {
    vector<vector<int>> scc;
    vector<int> in_stack;
    int timer = 1;
    for (int i = 1; i <= N; ++i) {
        if (disc[i] == -1) tarjan_dfs(i, timer, in_stack, scc);
    }
    // for (auto& vec : scc) sort(vec.begin(), vec.end());
    // sort(scc.begin(), scc.end());
    return scc;
}

// TODO: (SCC) Strongly Connected Components (DSU)

// TODO: complexities for all

// TODO : make timer global

// ----------------- code ----------------------

void solve() {
    // ---INPUT---
    cin >> N >> M;
    reset_graph(N);  // in case of multiple test cases
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
    // for (int i = 1; i <= N; ++i) topo_sort_DFS(i, topo_sort);
    // reverse(topo_sort.begin(), topo_sort.end());
    // bool has_cycle = (int) topo_sort.size() == N;

    // bool has_cycle = 0;
    // for (int i = 1; i <= N && !has_cycle; ++i) {
    //     if (vis[i]) continue;
    //     has_cycle |= has_cycle_DFS_undir(i);
    //     has_cycle |= has_cycle_BFS_undir(i);
    //     has_cycle |= has_cycle_DFS_dir(i);
    // }
    // bool has_cycle = has_cycle_BFS_dir();

    // ---PATHS, CIRCUITS---
    bool is_conn = is_euler_conn();
    bool euler_path = has_euler_path() && is_conn;
    bool euler_circuit = has_euler_circuit() && is_conn;
    bool ham_path = has_ham_path();
    bool ham_cycle = has_ham_cylce();

    // ---MST---
    // auto [tot_cst, mst_edges] = prim_mst();
    // auto [tot_cst, mst_edges] = kruskal_mst(edges);

    // ---BRIDGES---
    // vector<Edge> bridges;
    // int timer = 1;
    // for (int i = 1; i <= N; ++i) {
    //     if (disc[i] == -1) tarjan_bridges(i, -1, timer, bridges);
    // }

    // vector<int> cut_pts;
    // int timer = 1;
    // for (int i = 1; i <= N; ++i) {
    //     if (disc[i] == -1) tarjan_aps(i, -1, timer, cut_pts);
    // }
    // sort(cut_pts.begin(), cut_pts.end());

    // ---SCC---
    // vector<vector<int>> scc = kosaraju_scc(edges);
    // vector<vector<int>> scc = tarjan_scc();
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T = 1;
    // cin >> T;
    while (T--) solve();
}

// ----------------- notes ---------------------
/*
WHY to use dist[]?
 - avoid pushing the same node multiple times (extra pushes)
 - store shortest distances for all nodes

topological sort
 - linear ordering of vertices/nodes such that u comes before v for every
directed edge u->v
 - only for DAG
 - multiple orderings are possible

bridges & aritculation points
 - disc[u] is the time when node `u` is first visited using DFS
 - low[u] : lowest/smallest disc[x] (discovery time) reachable from node `u`
 - bridges (disc[parent] < low[child])
   - explicit back edge handling doesn't give wrong results but better to be
     safe than sorry
   - this means we do not get wrong results for using low[v] instead of
disc[v] separately
 - articulation points (disc[parent] <= low[child])
   - explicit back edge handling is REQUIRED
   - we should update low[u] with min(low[u], disc[v]) for back edge to get
     correct results
   - WHY? try out for this graph (1-4, 1-2, 1-3, 2-3, 2-5, 2-6, 5-7, 6-7)


checkpoints
- iterate over all components?
- build properly - directed / undirected?
- proper usage of long long (instead of int)?
- INF set to 4e18 (instead of 1e9)?
- proper init of arrs like parent, dist, ...?
- 0-based/1-based complete conversion (choose only one among two)?
- reset graph / data structures after each test case or in case of multiple
usage of same entity ?
- multiple / duplicate edges between two nodes?

non-intuitive ideas
- negate weights -> maximise becomes minimise or minimise becomes maximise
- invert direction of the edges (for directed) -> path from 1->N becomes
N->1
- spread bad nodes completely (mark nodes via BFS/DFS)
*/

// ----------------- archive ---------------------
/*
// TODO: check correctness (if wrong? don't know why, maybe fails for cycles?)
void topo_sort_DFS(int u, vector<int>& topo_sort) {
    if (vis[u]) return;
    vis[u] = 1;
    topo_sort.push_back(u);
    for (auto& [v, w] : g[u]) {
        if (--indeg[v] == 0) topo_sort_DFS(v, topo_sort);
    }
}
vector<int> topo_sort;
for (int i = 1; i <= N; ++i) {
    if (indeg[i] == 0) topo_sort_DFS(i, topo_sort);
}

// Timer function
void timer_dfs(int u, int p, int& d) {  // d=timer
    disc[u] = low[u] = d++;
    for (auto& [v, w] : g[u]) {
        if (v == p) continue;
        if (disc[v] == -1) timer_dfs(v, u, d);
        low[u] = min(low[u], low[v]);
    }
}

// Bridges - might lead to double counting of edges in case if there are
// multiple edges between two nodes
vector<Edge> get_bridges(vector<Edge> edges) {
    vector<Edge> bridges;
    int timer = 1;
    for (int i = 1; i <= N; ++i) {
        if (disc[i] == -1) timer_dfs(i, -1, timer);
    }
    for (auto e : edges) {
        int u = e.u, v = e.v;
        if (disc[u] < low[v] || disc[v] < low[u]) {
            bridges.push_back(e);
        }
    }
    return bridges;
}

// (loki's version) Bridges + Timer (concise & combined with timer)
// doesn't handle back edge explicitly, not standard tarjan's
void bridge_dfs(int u, int p, int& t, vector<Edge>& bridges) {
    disc[u] = low[u] = t++;
    for (auto& [v, w] : g[u]) {
        if (v == p) continue;
        if (disc[v] == -1) bridge_dfs(v, u, t, bridges);
        low[u] = min(low[u], low[v]);
        if (disc[u] < low[v]) bridges.push_back(Edge(u, v, w));
    }
}

vector<Edge> bridges = get_bridges(edges); // double counting!!!
vector<int> cut_pts = cut_points(); // TODO
*/