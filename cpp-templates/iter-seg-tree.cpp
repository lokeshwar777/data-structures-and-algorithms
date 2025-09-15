/* 777 */
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9 + 5;  // 4e18;
const int NEUTRAL = INF;  // INF for min, -INF for max, 0 for gcd/sum/XOR

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

    void build(vector<int> &vec) {                          // 0-based vector
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
            cin >> i >> x;
            --i;                    // if i is 1-based
            ST.point_update(i, x);  // i -> 0..N-1
        } else {
            cin >> l >> r;
            --l;
            --r;                       // if l & r are 1-based
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
*/

//        1
//    2       1
//  2   4   1   3
// 3 2 4 5 1 1 5 3
