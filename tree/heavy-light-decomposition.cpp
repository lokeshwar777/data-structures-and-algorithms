/* 777 */

#include <bits/stdc++.h>
using namespace std;
// #define int long long

const int MAX_N = 2e6 + 5, root = 1, INF = 1e9 + 5;  // 4e18;

// ----------------- Segment Tree on HLD ----------------------
class SegmentTree {
   private:
    vector<int> arr;
    vector<int> tree;

    int merge(int a, int b) {
        int res = max(a, b);
        return res;
    }

   public:
    SegmentTree(vector<int>& nums) {
        this->arr = nums;
        this->tree.resize(4 * nums.size(), 0);
    }

    void build(int v, int lo, int hi) {
        if (lo == hi) {
            tree[v] = arr[lo];
            return;
        }
        int mid = lo + (hi - lo) / 2;
        build(2 * v, lo, mid);
        build(2 * v + 1, mid + 1, hi);
        tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
    }

    void update(int v, int lo, int hi, int i, int x) {
        if (lo == hi) {
            tree[v] = x;
            return;
        }
        int mid = lo + (hi - lo) / 2;
        if (i <= mid) {
            update(2 * v, lo, mid, i, x);
        } else {
            update(2 * v + 1, mid + 1, hi, i, x);
        }
        tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
    }

    int query(int v, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return -INF;
        if (l <= lo && hi <= r) return tree[v];
        int mid = lo + (hi - lo) / 2;
        return merge(query(2 * v, lo, mid, l, min(mid, r)),
                     query(2 * v + 1, mid + 1, hi, max(l, mid + 1), r));
    }
};

// ---------------- declare ----------------------

int N, timer, parent[MAX_N], sub_sz[MAX_N], dep[MAX_N];
int vals[MAX_N], heavy[MAX_N], head[MAX_N], pos[MAX_N];
vector<int> tree[MAX_N];  // 1-based
SegmentTree* ST;          // 0-based

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
    delete ST;
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
        res = max(res, ST->query(1, 0, N - 1, l - 1, r - 1));  // 0-based
        v = parent[head[v]];
    }
    int l = pos[u], r = pos[v];
    if (l > r) swap(l, r);
    // add query(l, r) in segment tree to result
    res = max(res, ST->query(1, 0, N - 1, l - 1, r - 1));  // 0-based
    return res;
}

void node_update(int u, int x) {
    ST->update(1, 0, N - 1, pos[u] - 1, x);
}

// TODO: use lazy segment tree for range update
void path_update(int u, int v, int x) {
    while (head[u] != head[v]) {
        if (dep[head[u]] > dep[head[v]]) swap(u, v);
        int l = pos[head[v]], r = pos[v];
        // TODO: update(l, r) with x using lazy segment tree
        v = parent[head[v]];
    }
    int l = pos[u], r = pos[v];
    if (l > r) swap(l, r);
    // TODO: update(l, r) with x using lazy segment tree
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
    ST = new SegmentTree(vals_vec);
    ST->build(1, 0, N - 1);

    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, u, v, x;  // u=node, v=node, x=update value
        cin >> ty >> u;
        if (ty == 1) {  // update
            cin >> x;
            node_update(u, x);

            // cin >> v >> x;
            // path_update(u, v, x);
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

/*
- compute subtree sizes
- heavy child -> child having max sub_sz among all children, any in case of tie
- light chidren -> which are not heavy
- any path can be cut into atmost O(logN) chains -> WHY?
    - we call a child heavy if it has max sub_sz
    - if it is max then it will have atleast sub_sz(parent)/2
    - sub_sz(heavy child) >= sub_sz(parent)/2 and sub_sz(light child) <=
sub_sz(parent)/2
    - so this process happens for atmost logN times
- chain = contiguous block
- heavy chain -> chain formed using only heavy nodes (different for each path)
- light chains -> rest all branching out from this heavy chain to remaining leaf
nodes (new chains)

(TODO)
- mark heavy and heads
- create chains
- mark position using timer
- query
- update

- safe practices
    - pos[v] != -1 maybe not safer (inside make_chains children exploration)
*/