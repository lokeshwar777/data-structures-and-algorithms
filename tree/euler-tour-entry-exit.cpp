/* 777 */

#include <bits/stdc++.h>
using namespace std;
// #define int long long

const int MAX_N = 2e6 + 5, root = 1;
// LCA
int dep[MAX_N], first[MAX_N];
vector<int> euler;

// ---------------- Sparse Table -------------
int highestPowerOf2(uint32_t x) {
    return 31 - __builtin_clz(x);
}
class SparseTable {
    int N, L;
    vector<vector<int>> table;

   public:
    SparseTable(int n) {
        this->N = n;
        this->L = highestPowerOf2(n) + 1;
        this->table.resize(n, vector<int>(this->L, -1));
        // table[i][k] -> stores the index in the euler tour which has minimum
        // depth of `2^k` elements from `i`
    }

    void build() {
        for (int i = 0; i < N; ++i) {
            table[i][0] = i;
        }
        for (int k = 1; k < L; ++k) {
            for (int i = 0; i < N - (1 << k) + 1; ++i) {
                int x = table[i][k - 1], y = table[i + (1 << (k - 1))][k - 1];
                int dx = dep[euler[x]], dy = dep[euler[y]];
                table[i][k] = dx < dy ? x : y;
            }
        }
    }

    int query(int l, int r) {
        int k = highestPowerOf2(r - l + 1);
        int x = table[l][k], y = table[r - (1 << k) + 1][k];
        int dx = dep[euler[x]], dy = dep[euler[y]];
        return dx < dy ? x : y;
    }
};

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

// ---------------- declare ----------------------

int N, timer;
int in[MAX_N], out[MAX_N], vals[MAX_N], flat[2 * MAX_N];
vector<int> tree[MAX_N];  // 1-based
SparseTable* SPT;         // 0-based
FenwickTree* FT;          // 1-based

// ---------------- build ----------------------

void add_edge(int u, int v) {
    tree[u].push_back(v);
    tree[v].push_back(u);
}

void reset_tree(int n) {
    timer = 0;
    for (int i = 0; i <= n; ++i) {
        tree[i].clear();
        in[i] = out[i] = vals[i] = flat[i] = flat[n + i] = dep[i] = 0;
        first[i] = -1;
    }
    euler.clear();
    delete SPT;
    delete FT;
}

// ----------------- 2N ETT ---------------------

// assign in (entry) and out (exit) times
void euler_dfs(int u = root, int p = -1) {
    in[u] = ++timer;  // increase timer at entry
    if (p != -1) dep[u] = dep[p] + 1;
    if (first[u] == -1) first[u] = (int) euler.size();
    euler.push_back(u);
    for (int v : tree[u]) {
        if (v == p) continue;
        euler_dfs(v, u);
        euler.push_back(u);
    }
    out[u] = ++timer;  // increase timer at exit (unique times)
}

// subtree/ancestor check in O(1), anc=ancestor, des=descendant
bool is_ancestor(int anc, int des) {
    return in[anc] < in[des] && out[des] < out[anc];
}

// subtree size in O(1)
int get_sub_sz(int u) {
    return (out[u] - in[u] + 1) >> 1;
}

// flatten into array (base array) in O(N)
void flatten() {
    for (int u = 1; u <= N; ++u) {
        flat[in[u]] = vals[u];

        // cancelling out
        flat[out[u]] = -vals[u];  // sum
        // flat[out[u]] = vals[u];  // XOR
        // flat[out[u]] = 0;  // gcd
    }
}

// ----------------- operations ----------------------

// update the value of node `u` to x=new value
void node_update(int u, int x) {
    int i = in[u], j = out[u];
    FT->update(i, i, (x - flat[i]));  // 1-based
    FT->update(j, j, -x - flat[j]);   // 1-based
    flat[i] = x;
    flat[j] = -x;  // sum
    // flat[j] = x; // xor
}

// perform query over path from root to b
int root_query(int u) {
    int l = in[root], r = in[u], res;
    res = FT->query(l, r);
    return res;
}

int lca(int a, int b) {
    int l = first[a], r = first[b];
    if (l > r) swap(l, r);
    int idx = SPT->query(l, r);  //  index of min(depth[l..r]) in euler[l..r]
    return euler[idx];
}

// perform query over path from a to b
int path_query(int a, int b) {
    int l = lca(a, b);
    return root_query(a) + root_query(b) - 2 * root_query(l) + flat[in[l]];
}

// ----------------- debug ----------------------

void print_DS() {
    for (int i = 1; i <= N; ++i) cout << i << " ";
    cout << "\nin  ";
    for (int i = 1; i <= N; ++i) cout << in[i] << " ";
    cout << "\nout ";
    for (int i = 1; i <= N; ++i) cout << out[i] << " ";
    cout << "\ndep ";
    for (int i = 1; i <= N; ++i) cout << dep[i] << " ";
    cout << "\neul ";
    for (int x : euler) cout << x << " ";
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

    // ---LCA---
    SPT = new SparseTable((int) euler.size());
    SPT->build();

    // ---Path Queries (invertible=sum/XOR/...)---
    FT = new FenwickTree(2 * N);
    for (int i = 1; i <= 2 * N; ++i) FT->update(i, i, flat[i]);

    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, u, v = root, n_val;  // u=node, v=node, n_val=new value
        cin >> ty >> u;
        if (ty == 1) {  // update
            cin >> n_val;
            node_update(u, n_val);
        } else if (ty == 2) {  // query
            // cin >> v;
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

// archive
/*
// baseline version of tree flattening into an array
// tweak / make changes to out[u]th index based on what the problem is asking
void flatten() {
    for (int u = 1; u <= N; ++u) {
        flat[in[u]] = vals[u];
        flat[out[u]] = vals[u];
    }
}
*/