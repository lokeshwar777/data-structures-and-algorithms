/* 777 */

#include <bits/stdc++.h>
using namespace std;
// #define int long long

const int MAX_N = 2e6 + 5, root = 1, INF = 1e9 + 5;  // 4e18;
const int NEUTRAL = INF;  // INF for min, -INF for max, 0 for gcd/sum/XOR

// ----------------- Segment Tree on HLD ----------------------
struct segTree {  // 1-based segment tree
   private:
    vector<int> seg;  // seg=flat tree
    int sz;

    int merge(int a, int b) {
        int res = min(a, b);
        return res;
    }

   public:
    segTree() {}  // default constructor

    segTree(int n) {  // root of segment tree it is at seg[1]
        sz = n;       // N=sz
        seg.resize(2 * sz, NEUTRAL);
    }

    void build(vector<int>& vec) {                          // 0-based vector
        for (int i = 0; i < sz; ++i) seg[sz + i] = vec[i];  // N .. 2N-1
        for (int i = sz - 1; i; --i)
            seg[i] = merge(seg[2 * i], seg[2 * i + 1]);  // 1 .. N-1
    }

    void point_update(int ii, int x) {  // ii -> 0 .. N-1
        seg[sz + ii] = x;
        for (int i = (sz + ii) >> 1; i; i >>= 1)
            seg[i] = merge(seg[2 * i], seg[2 * i + 1]);
    }

    int query(int ll, int rr) {  // (ll, rr) -> 0..N-1
        int res = NEUTRAL;
        for (int l = sz + ll, r = sz + rr; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) res = merge(res, seg[l++]);
            if (!(r & 1)) res = merge(res, seg[r--]);
        }
        return res;
    }
};

// ---------------- declare ----------------------

int N, timer, parent[MAX_N], sub_sz[MAX_N], dep[MAX_N];
int vals[MAX_N], heavy[MAX_N], head[MAX_N], pos[MAX_N];
vector<int> tree[MAX_N];  // 1-based
segTree ST;               // 0-based

// ---------------- build ----------------------

void add_edge(int u, int v) {
    tree[u].push_back(v);
    tree[v].push_back(u);
}

void reset_tree(int n) {
    timer = 0;
    for (int i = 0; i <= n; ++i) {
        tree[i].clear();
        sub_sz[i] = dep[i] = vals[i] = 0;
        heavy[i] = pos[i] = parent[i] = head[i] = -1;
    }
}

// ----------------- HLD ----------------------

void mark_heavy_dfs(int u = root, int p = -1) {
    sub_sz[u] = 1;
    int hc = -1;  // hc=heavy child
    if (p != -1) dep[u] = dep[p] + 1;
    parent[u] = p;
    for (int v : tree[u]) {
        if (v == p) continue;
        mark_heavy_dfs(v, u);
        sub_sz[u] += sub_sz[v];
        if (hc == -1 || sub_sz[v] > sub_sz[hc]) hc = v;
    }
    heavy[u] = hc;
}

void make_chains(int u = root, int p = -1, int hd = root) {
    head[u] = hd;
    pos[u] = ++timer;
    if (heavy[u] != -1) make_chains(heavy[u], u, hd);
    for (int v : tree[u]) {
        if (v == p || v == heavy[u]) continue;
        make_chains(v, u, v);
    }
}

int path_query(int u, int v) {
    int res = -INF;
    while (head[u] != head[v]) {
        if (dep[head[u]] > dep[head[v]]) swap(u, v);
        int l = pos[head[v]], r = pos[v];
        // add query(l, r) in segment tree to result
        res = max(res, ST.query(l - 1, r - 1));  // (l,r) -> 0..N-1
        v = parent[head[v]];
    }
    int l = pos[u], r = pos[v];
    if (l > r) swap(l, r);
    // add query(l, r) in segment tree to result
    res = max(res, ST.query(l - 1, r - 1));  // (l,r) -> 0..N-1
    return res;
}

void node_update(int u, int x) {
    ST.point_update(pos[u] - 1, x);  // i -> 0..N-1
}

// ----------------- debug ----------------------

void print_DS() {
    for (int i = 1; i <= N; ++i) cout << i << " ";
    cout << "\npar ";
    for (int i = 1; i <= N; ++i) cout << parent[i] << " ";
    cout << "\nhvy ";
    for (int i = 1; i <= N; ++i) cout << heavy[i] << " ";
    cout << "\nhd  ";
    for (int i = 1; i <= N; ++i) cout << head[i] << " ";
    cout << "\npos ";
    for (int i = 1; i <= N; ++i) cout << pos[i] << " ";
    cout << endl;
}

// ----------------- code ----------------------

void solve() {
    int Q;
    cin >> N >> Q;

    reset_tree(N);  // in case of multiple test cases

    for (int i = 1; i <= N; ++i) cin >> vals[i];
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        // u++;v++ // for 0-based nodes
        add_edge(u, v);
    }

    // --HLD
    mark_heavy_dfs();
    make_chains();

    // Path Queries (non-invertible=min/max/gcd/...)
    vector<int> vals_vec(N);
    for (int i = 1; i <= N; ++i) vals_vec[pos[i] - 1] = vals[i];
    ST = segTree(N);  // root=1
    ST.build(vals_vec);

    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, u, v, x;  // u=node, v=node, x=update value
        cin >> ty >> u;
        if (ty == 1) {  // update
            cin >> x;
            node_update(u, x);
        } else if (ty == 2) {  // query
            cin >> v;
            int res = path_query(u, v);
            qs.push_back(res);
        }
    }

    for (int x : qs) cout << x << '\n';

    // ---DEBUG---
    // print_DS();
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T = 1;
    // cin >> T; // multiple test cases
    while (T--) solve();
    return 0;
}