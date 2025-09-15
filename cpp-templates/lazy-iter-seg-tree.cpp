/* 777 */
#include <bits/stdc++.h>
using namespace std;
// #define int long long

const int INF = 1e9 + 5;  // 4e18;
const int NEUTRAL = INF;  // INF for min, -INF for max, 0 for gcd/sum/XOR

struct segTree {  // 1-based segment tree
   private:
    vector<int> SEG, ADD;  // SEG=flat tree
    int sz;

    int merge(int a, int b) {
        int res = min(a, b);
        return res;
    }

   public:
    segTree() {}  // default constructor

    segTree(int n) {  // root of segment tree it is at SEG[1]
        sz = n;       // N=sz
        SEG.resize(2 * sz, NEUTRAL);
        ADD.resize(2 * sz, 0);
    }

    void build(vector<int> &vec) {                          // 0-based vector
        for (int i = 0; i < sz; ++i) SEG[sz + i] = vec[i];  // N .. 2N-1
        for (int i = sz - 1; i; --i)
            SEG[i] = merge(SEG[2 * i], SEG[2 * i + 1]);  // 1 .. N-1
    }

    void point_update(int ii, int x) {  // ii -> 0 .. N-1
        SEG[sz + ii] = x;
        for (int i = (sz + ii) >> 1; i; i >>= 1)
            SEG[i] = merge(SEG[2 * i], SEG[2 * i + 1]);
    }

    void push_down(int ii) {  // ii -> leaf node
        vector<int> ancs;     // ancs=ancestors
        for (int i = ii >> 1; i; i >>= 1) ancs.push_back(i);
        int n = ancs.size();
        for (int j = n - 1, K = 1 << n; j >= 0; --j, K >>= 1) {
            int i = ancs[j];
            apply_add(2 * i, ADD[i], K >> 1);
            apply_add(2 * i + 1, ADD[i], K >> 1);
            ADD[i] = 0;
        }
    }

    void pull_up(int ii, int K) {
        ii >>= 1, K <<= 1;  // leaf nodes do not have children
        for (int i = ii; i; i >>= 1, K <<= 1) {
            SEG[i] = merge(SEG[2 * i], SEG[2 * i + 1]);
            // + ADD[i] * K; // (for sum segment tree)
        }
    }

    void apply_add(int ii, int x, int K) {
        SEG[ii] += x;
        // += x * K; // (for sum segment tree)
        if (ii < sz) ADD[ii] += x;
    }

    void range_update(int ll, int rr, int x) {  // (ll, rr) -> 0..N-1
        ll += sz, rr += sz;
        push_down(ll), push_down(rr);

        for (int l = ll, r = rr, K = 1; l <= r; l >>= 1, r >>= 1, K <<= 1) {
            if (l & 1) apply_add(l++, x, K);
            if (!(r & 1)) apply_add(r--, x, K);
        }

        pull_up(ll, 1), pull_up(rr, 1);
    }

    void range_change(int ll, int rr, int x) {  // (ll, rr) -> 0..N-1
    }

    int query(int ll, int rr) {  // (ll, rr) -> 0..N-1
        ll += sz, rr += sz;
        push_down(ll), push_down(rr);

        int res = NEUTRAL;
        for (int l = ll, r = rr; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) res = merge(res, SEG[l++]);
            if (!(r & 1)) res = merge(res, SEG[r--]);
        }
        return res;
    }
};

void solve() {
    int N, Q;
    cin >> N >> Q;
    vector<int> arr(N);
    for (int &x : arr) cin >> x;

    segTree ST(N);  // root=1
    ST.build(arr);

    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, l, r, i, x;
        cin >> ty;
        if (ty == 1) {
            // cin >> i >> x;
            // --i;                    // if i is 1-based
            // ST.point_update(i, x);  // i -> 0..N-1
            // NOTE: point update does not use lazy

            cin >> l >> r >> x;
            // --l, --r;                  // if l & r are 1-based
            ST.range_update(l, r, x);  // (l,r) -> 0..N-1
        } else if (ty == 2) {
            cin >> l >> r >> x;
            // --l, --r;  // if l & r are 1-based
            ST.range_change(l, r, x);
        } else if (ty == 3) {
            cin >> l >> r;
            // --l, --r;                  // if l & r are 1-based
            int res = ST.query(l, r);  // (l,r) -> 0..N-1
            qs.push_back(res);
        }
    }

    for (int x : qs) cout << x << '\n';
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T = 1;
    // cin >> T; // multiple test cases
    while (T--) solve();
}

/*
- remove merge if you want to reduce overhead
- trick: do not think this as a complete binary tree instead think as
parent-child rlship

---- 0-based ----
- root can be at 1 or 0
- (next power of 2, 4N is safer) nodes/size of flattened array
- if parent is at i, left child is at 2 * i + 1, right child is at 2 * i + 2
- if child is at i, parent is at floor((i - 1) / 2)
- if i is odd then it is a left child
- if i is even then it is a right child
- [1..N-1] -> parents/constructed nodes
- [N..2*N] -> leaf nodes/simply values of array

- complex math for parent-child relationship
- requires a complete tree (next power of 2 for size of array, 4N-safe)
- good for recursive

---- 1-based (root=1) ----
- root is at 1 (can also be at 0 but ..)
- 0th index of flattened array is unused if not updates & queries get messier
- 2 * N nodes/size of flattened array
- if parent is at i, left child is at 2 * i, right child is at 2 * i + 1
- if child is at i, parent is at floor(i / 2)
- if i is even then it is a left child
- if i is odd then it is a right child
- [1..N-1] -> parents/constructed nodes
- [N..(2*N - 1)] -> leaf nodes/simply values of array

- simple math for parent-child relationship
- requires just 2N nodes
- good for iterative

2N nodes case:
    so difference between even and odd size is
    for even size all the parents (N>>1)+1 .. N-1
    right shift their arrows to their children
    like they had in case if N was even i.e N-1
    and N-1 becomes a parent from child

// TODO : lazy + corrections
*/
