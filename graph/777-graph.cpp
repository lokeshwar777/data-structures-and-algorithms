#include "bits/stdc++.h"
using namespace std;

struct Edge {
    int u, v, w;
    Edge(int from, int to, int weight) : u(from), v(to), w(weight) {}
};

const int MAX_N = 2e6 + 5, INF = 1e9 + 5;
int N, M, parent[MAX_N], path_vis[MAX_N], indeg[MAX_N], colors[MAX_N];
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
    for (auto& e : edges) {
        add_edge(e.u, e.v, e.w, dir);  // (from, to, weight, isDirected)
    }
    for (int i = 1; i <= N; ++i) colors[i] = parent[i] = -1;
}

void reset_graph() {
    for (int i = 1; i <= N; ++i) {
        g[i].clear();
        path_vis[i] = indeg[i] = 0;
        colors[i] = parent[i] = -1;
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

// ----------------- coloring ---------------------

// colors should be initially not set i.e -1
bool coloring_DFS(int u, int p = -1, int cur = 0) {
    if (colors[u] != -1 && colors[u] != cur) return 0;
    if (colors[u] != -1) return 1;
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

// ----------------- code ----------------------

void solve() {
    // ---INPUT---
    cin >> N >> M;
    vector<Edge> edges;
    edges.reserve(M);
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

    // ---CLEAN UP---
    // reset_graph();  // in case of multiple test cases
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

stay careful about connected components
*/