/* 777 */

#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 2e6 + 5, LOG = 20;

int up[MAX_N][LOG], dep[MAX_N];
vector<int> g[MAX_N];  // 1-based

void add_edge(int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
}

void build(int u, int p = -1) {
    up[u][0] = p;
    for (int k = 1; k < LOG; ++k) {
        up[u][k] = up[u][k - 1] != -1 ? up[up[u][k - 1]][k - 1] : -1;
    }
    for (int v : g[u]) {
        if (v == p) continue;
        dep[v] = 1 + dep[u];
        build(v, u);
    }
}

int kth_ancestor(int u, int K) {
    for (int k = 0; k < LOG && u != -1; ++k) {
        if (K & (1 << k)) u = up[u][k];
    }
    return u;
}

int lca(int a, int b) {
    if (dep[a] < dep[b]) swap(a, b);
    a = kth_ancestor(a, dep[a] - dep[b]);
    if (a == b) return a;
    // lift both a and b until just before LCA
    for (int k = LOG - 1; k >= 0; --k) {
        if (up[a][k] != up[b][k]) {
            a = up[a][k];
            b = up[b][k];
        }
    }
    return up[a][0];  // parent is LCA
}

int distance(int a, int b) {
    int x = lca(a, b);  // x-lowest common ancestor of a & b
    // (depth[a] - depth[lca(a, b)]) + (depth[b] - depth[lca(a, b)])
    return dep[a] + dep[b] - 2 * dep[x];
}

void solve() {
    // 1. Take Input
    int N, Q;
    cin >> N >> Q;
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        // u++;v++ // for 0-based nodes
        add_edge(u, v);
    }

    // 2. Build the 2D parent matrix using binary lifting
    build(1);  // rooted at 1

    // 3. Perform queries
    vector<int> qs(Q);
    for (int i = 0, a, b; i < Q; ++i) {
        cin >> a >> b;
        int res = distance(a, b);
        // int res = kth_ancestor(a, b); // u=a, k=b
        // int res = lca(a, b);
        qs[i] = res;
    }

    // for (int u = 1; u <= N; ++u) {
    //     cout << u << " ";
    //     for (int k = 0; k < LOG; ++k) cout << up[u][k] << " ";
    //     cout << endl;
    // }

    for (int q : qs) cout << q << '\n';
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}

/*
root the g and precompute the props of ancestors using binary lifting

for each node we compute up[node][2^kth ancestor] where k from 0 to 31
up[node][0] = up
for k: 1->31
    (if up[node][k - 1] exists)
    up[node][k] = up[up[node][k - 1]][k - 1]
if you want kth power at the current node, go to the node at k-1th power and
ask it who is the node at k-1th power from you
*/