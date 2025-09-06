#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 2e6 + 5;
int N, par[MAX_N], sz[MAX_N], rk[MAX_N];
// par-parent, sz-size, rk-rank

// naive DSU
/*
// O(N) returns ultimate parent of u
int find(int u) {
    while (par[u] != u) u = par[u];
    return u;
}

// O(N) performs union of group of a and group of b
void merge(int a, int b) {
    a = find(a);
    b = find(b);
    par[a] = b;
}
*/

// uses path compression O(⍺(N)) - inverse Ackermann function
// returns ultimate parent of u
int find(int u) {
    return par[u] = (u == par[u]) ? u : find(par[u]);
}

// O(logN) - height of tree, O(⍺(N)) with path compression
// merge group a into group b (assuming a has smaller size than b)
void union_by_size(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return;
    if (sz[a] > sz[b]) swap(a, b);
    par[a] = b;
    sz[b] += sz[a];
}

// O(logN) - height of tree, O(⍺(N)) with path compression
// merge group a into group b (assuming a has smaller rank than b)
void union_by_rank(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return;
    if (rk[a] > rk[b]) swap(a, b);
    par[a] = b;
    rk[b] += rk[a] == rk[b];
}

// returns whether a and b belong to the same group
bool same(int a, int b) {
    return find(a) == find(b);
}

void init() {
    for (int i = 1; i <= N; ++i) {
        par[i] = i;
        sz[i] = 1;
        rk[i] = 0;
    }
}

void solve() {
    int Q;
    cin >> N >> Q;
    init();  // O(N)
    // find (path compression) + union (size/rank) -> O(⍺(N))
    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, a, b;
        cin >> ty >> a >> b;
        // a++; b++; // 0-based
        if (ty) {  // a is connected with b?
            int res = same(a, b);
            qs.push_back(res);
        } else {  // union(a,b)
            // use either size or rank (!!! NOT BOTH !!!)
            union_by_size(a, b);
            // union_by_rank(a, b);
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

/*
PATH COMPRESSION
  - converts O(logN) of height in the worst case -> O(1)
  - it basically flattens the tree

UNION BY SIZE/RANK
  - prevent tree from growing tall
  - reduces find() operatioins

UNION BY SIZE
  - root represents set size (no. of elements in its group)
  - amortized O(⍺(N)),
  - merge smaller sizes into larger sizes (parent[small] = large)
  - update root having larger size (large += small)

UNION BY RANK
  - root represents rank (kind of height/depth approx. for simplicity)
  - amortized O(⍺(N)), memory efficient
  - merge smaller rank/height into larger rank/height (parent[small] = large)
  - increment root having higher height only when both the ranks are same
    (large++)

both provide the same functionality,
    size -> simple, easy to debug & visualise
    rank -> it's complicated, confusing, ...
*/