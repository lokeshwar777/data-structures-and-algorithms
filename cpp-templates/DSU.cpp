#include <bits/stdc++.h>
using namespace std;

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

void solve() {
    int N, Q;
    cin >> N >> Q;
    DSU dsu(N);
    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, a, b;
        cin >> ty >> a >> b;
        // a++; b++; // 0-based
        if (ty) {  // a is connected with b?
            int res = dsu.is_conn(a, b);
            qs.push_back(res);
        } else {  // union(a,b)
            dsu.union_sz(a, b);
        }
    }
    for (int x : qs) cout << x << "\n";
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T = 1;
    // cin >> T;
    while (T--) solve();
}