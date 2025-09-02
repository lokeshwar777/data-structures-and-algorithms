/* 777 */

#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 2e6 + 5;

int dep[MAX_N], par[MAX_N], sub_sz[MAX_N], dist[MAX_N];
vector<int> tree[MAX_N];  // 1-based

void add_edge(int u, int v) {
    tree[u].push_back(v);
    tree[v].push_back(u);
}

int dfs(int u, int p = -1) {
    int ans = 0;
    par[u] = p;
    sub_sz[u] = 1;
    for (int v : tree[u]) {
        if (v == p) continue;
        dep[v] = dep[u] + 1;
        dfs(v, u);
        sub_sz[u] += sub_sz[v];
    }
    return ans;
}

int height(int root, int n) {
    dfs(root);
    int ht = 0;
    for (int i = 1; i <= n; ++i) ht = max(ht, dep[i]);
    return ht;
}

pair<int, int> farthest(int root, int root_par = -1) {
    int best = root;
    dist[best] = 0;
    queue<pair<int, int>> q;
    q.emplace(root, root_par);
    while (!q.empty()) {
        auto [u, p] = q.front();
        q.pop();
        if (dist[u] > dist[best]) best = u;
        for (int v : tree[u]) {
            if (v == p) continue;
            dist[v] = dist[u] + 1;
            q.emplace(v, u);
        }
    }
    return {dist[best], best};
}

int diameter() {
    auto [ht, fn] = farthest(1);    // using BFS
    auto [diam, _] = farthest(fn);  // using BFS
    return diam;
}

void reset_tree(int n) {
    for (int i = 0; i <= n; ++i) {
        tree[i].clear();
        dep[i] = sub_sz[i] = par[i] = dist[i] = 0;
    }
}

void solve() {
    int N;
    cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        // u++;v++ // for 0-based nodes
        add_edge(u, v);
    }

    // int res = dfs(1);
    // int ht = height(1, N);
    // int diam = diameter();

    // reset_tree(N);  // in case of multiple test cases
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}