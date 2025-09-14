/* 777 */

#include <bits/stdc++.h>
using namespace std;
// #define int long long

// ----------------- BIT/Fenwick Tree on ETT ----------------------
class FenwickTree {
   private:
    int N;
    vector<int> B1, B2;  // 1-based

    int _query(vector<int>& B, int i) {
        int s = 0;
        for (; i; i -= i & -i) s += B[i];
        return s;
    }

    void _update(vector<int>& B, int i, int d) {
        for (; i < N; i += i & -i) B[i] += d;
    }

   public:
    FenwickTree(int n) {
        N = n + 1;
        B1.resize(N, 0);
        B2.resize(N, 0);
    }

    void update(int l, int r, int d) {
        _update(B1, l, d);
        _update(B1, r + 1, -d);
        _update(B2, l, -d * (l - 1));
        _update(B2, r + 1, d * r);
    }

    int prefSum(int i) { return _query(B1, i) * i + _query(B2, i); }

    int query(int l, int r) { return prefSum(r) - prefSum(l - 1); }
};

// ----------------- Segment Tree on ETT ----------------------
class SegmentTree {
   private:
    vector<int> arr;
    vector<int> tree;

    int merge(int a, int b) {
        int res = a + b;
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
        if (l > r) return 0;
        if (lo == l && hi == r) return tree[v];
        int mid = lo + (hi - lo) / 2;
        return merge(query(2 * v, lo, mid, l, min(mid, r)),
                     query(2 * v + 1, mid + 1, hi, max(l, mid + 1), r));
    }
};

// ---------------- declare ----------------------

const int MAX_N = 2e6 + 5, root = 1;
int N, in[MAX_N], out[MAX_N], timer, vals[MAX_N], flat[MAX_N];
vector<int> tree[MAX_N];  // 1-based
FenwickTree* FT;          // 1-based
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
        in[i] = out[i] = vals[i] = flat[i] = 0;
    }
    delete FT;
    delete ST;
}

// ----------------- 1N ETT ---------------------

// assign in (entry) and out (exit) times
void euler_dfs(int u = root, int p = -1) {
    in[u] = ++timer;  // increase timer at entry
    for (int v : tree[u]) {
        if (v != p) euler_dfs(v, u);
    }
    out[u] = timer;
}

// subtree/ancestor check in O(1), anc=ancestor, des=descendant
bool is_ancestor(int anc, int des) {
    return in[anc] <= in[des] && out[des] <= out[anc];
}

// subtree size in O(1)
int get_sub_sz(int u) {
    return out[u] - in[u] + 1;
}

// flatten into array (base array) in O(N)
void flatten() {
    for (int u = 1; u <= N; ++u) {
        flat[in[u]] = vals[u];
    }
}

// ----------------- operations ----------------------

// update the value of node `u` to x=new value
void node_update(int u, int x) {
    int i = in[u];

    // FT->update(i, i, x - flat[i]);  // 1-based
    // or
    // ST->update(1, 0, N - 1, i - 1, x);  // 0-based

    flat[i] = x;
}

// perform query on subtree rooted at `u`
int subtree_query(int u) {
    int l = in[u], r = out[u], res;

    // res = FT->query(l, r);
    // or
    // res = ST->query(1, 0, N - 1, l - 1, r - 1);  // 0-based

    return res;
}

// ----------------- debug ----------------------

void print_DS() {
    for (int i = 1; i <= N; ++i) cout << i << " ";
    cout << "\nin  ";
    for (int i = 1; i <= N; ++i) cout << in[i] << " ";
    cout << "\nout ";
    for (int i = 1; i <= N; ++i) cout << out[i] << " ";
    cout << "\nflt ";
    for (int i = 1; i <= 2 * N; ++i) cout << flat[i] << " ";
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

    // ETT (Euler Tour in Trees) or Tree Flattening or In-Out - Subtree Queries
    euler_dfs(1, -1);
    flatten();

    // ---Range Query DS initialisation---

    // Subtree Queries (invertible=sum/XOR/...)
    /*
    FT = new FenwickTree(N);
    for (int i = 1; i <= N; ++i) FT->update(i, i, flat[i]);
    */

    // Subtree Queries (non-invertible=min/max/gcd/...)
    /*
    vector<int> flat_vec(flat + 1, flat + N + 1);
    ST = new SegmentTree(flat_vec);
    ST->build(1, 0, N - 1);
    */

    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, u, n_val,
            sub_rt;  // u=node, n_val= new value, sub_rt= subtree root
        cin >> ty;
        if (ty == 1) {  // update
            cin >> u >> n_val;
            node_update(u, n_val);
        } else if (ty == 2) {  // query
            cin >> sub_rt;
            int res = subtree_query(sub_rt);
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
