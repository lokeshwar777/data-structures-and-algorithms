/* 777 */
#include <iostream>
#include <vector>
using namespace std;

const int INF = 1e9 + 5;  // 4e18;
const int NEUTRAL = INF;  // INF for min, -INF for max, 0 for gcd/sum/XOR

struct LazySegmentTree {
   private:
    vector<int> arr, TREE, ADD, SET;
    vector<bool> CHANGED;

    int merge(int a, int b) {
        int res = min(a, b);
        return res;
    }

   public:
    LazySegmentTree() {}  // default constructor

    LazySegmentTree(vector<int> &nums) {
        arr = nums;
        TREE.resize(4 * nums.size(), NEUTRAL);
        ADD.resize(4 * nums.size(), 0);
        SET.resize(4 * nums.size(), 0);
        CHANGED.resize(4 * nums.size(), 0);
    }

    void build(int v, int lo, int hi) {
        if (lo == hi) {
            TREE[v] = arr[lo];
            return;
        }
        int mid = lo + (hi - lo) / 2;
        build(2 * v, lo, mid);
        build(2 * v + 1, mid + 1, hi);
        TREE[v] = merge(TREE[2 * v], TREE[2 * v + 1]);
    }

    void point_update(int v, int lo, int hi, int i, int x) {
        push(v, lo, hi);
        if (lo == hi) {
            TREE[v] = x;
            return;
        }
        int mid = lo + (hi - lo) / 2;
        if (i <= mid) {
            point_update(2 * v, lo, mid, i, x);
        } else {
            point_update(2 * v + 1, mid + 1, hi, i, x);
        }
        TREE[v] = merge(TREE[2 * v], TREE[2 * v + 1]);
    }

    void push(int v, int lo, int hi) {
        // SET > ADD - SET dominates increments(ADD) in child segments

        // SET lazy
        if (CHANGED[v]) {
            TREE[v] = SET[v];  // for sum tree -> SET[v] * (hi - lo + 1)
            if (lo != hi) {
                SET[2 * v] = SET[2 * v + 1] = SET[v];
                CHANGED[2 * v] = CHANGED[2 * v + 1] = 1;
                ADD[2 * v] = ADD[2 * v + 1] = 0;
            }
            SET[v] = CHANGED[v] = 0;
        }

        // ADD lazy
        if (ADD[v]) {
            TREE[v] += ADD[v];  // for sum tree -> ADD[v] * (hi - lo + 1)
            if (lo != hi) {
                ADD[2 * v] += ADD[v];
                ADD[2 * v + 1] += ADD[v];
            }
            ADD[v] = 0;
        }
    }

    void range_change(int v, int lo, int hi, int l, int r, int x) {
        push(v, lo, hi);
        if (r < lo || hi < l) return;  // invalid segment
        if (l <= lo && hi <= r) {      // completely inside/overlap
            SET[v] = x;
            CHANGED[v] = 1;
            push(v, lo, hi);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        range_change(2 * v, lo, mid, l, min(mid, r), x);
        range_change(2 * v + 1, mid + 1, hi, max(l, mid + 1), r, x);
        TREE[v] = merge(TREE[2 * v], TREE[2 * v + 1]);
    }

    void range_update(int v, int lo, int hi, int l, int r, int x) {
        push(v, lo, hi);
        if (r < lo || hi < l) return;  // invalid segment
        if (l <= lo && hi <= r) {      // completely inside/overlap
            ADD[v] += x;
            push(v, lo, hi);
            return;
        }
        int mid = lo + (hi - lo) / 2;
        range_update(2 * v, lo, mid, l, min(mid, r), x);
        range_update(2 * v + 1, mid + 1, hi, max(l, mid + 1), r, x);
        TREE[v] = merge(TREE[2 * v], TREE[2 * v + 1]);
    }

    int query(int v, int lo, int hi, int l, int r) {
        push(v, lo, hi);
        if (r < lo || hi < l) return NEUTRAL;
        if (l <= lo && hi <= r) return TREE[v];
        int mid = lo + (hi - lo) / 2;
        return merge(query(2 * v, lo, mid, l, min(mid, r)),
                     query(2 * v + 1, mid + 1, hi, max(l, mid + 1), r));
    }
};

void solve() {
    int N, Q;
    cin >> N >> Q;
    vector<int> arr(N);
    for (int &x : arr) cin >> x;

    LazySegmentTree ST = LazySegmentTree(arr);
    ST.build(1, 0, N - 1);

    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, l, r, i, x;
        cin >> ty;
        if (ty == 1) {  // update (increment/decrement-point/range)
            // set value at index `i` to value `x`
            // cin >> i >> x;
            // --i; // if i is 0-based;
            // ST.point_update(1, 0, N - 1, i, x);  // i -> 0..N-1
            // NOTE: point update is not using ADD

            // increase values in range `l` to `r` with `x`
            // cin >> l >> r >> x;
            // --l, --r; // if l & r are 1-based
            // ST.range_update(1, 0, N - 1, l, r, x);  // (l,r) -> 0..N-1
        } else if (ty == 2) {  // change (value-point/range)
            cin >> l >> r >> x;
            // --l, --r; // if l & r are 1-based
            // changes values in range `l` to `r` to `x`
            ST.range_change(1, 0, N - 1, l, r, x);  // (l,r) -> 0..N-1
        } else if (ty == 3) {                       // query (point/range)
            // perform query over range `l` to `r`
            cin >> l >> r;
            // --l, --r; // if l & r are 1-based
            int res = ST.query(1, 0, N - 1, l, r);  // (l,r) -> 0..N-1
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
