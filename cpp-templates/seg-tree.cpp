#include <iostream>
#include <vector>
using namespace std;

class SegmentTree {
   private:
    vector<int> arr;
    vector<int> tree;

    int merge(int a, int b) {
        int res = a + b;
        return res;
    }

   public:
    SegmentTree(vector<int> &nums) {
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

void solve() {
    int N, Q;
    cin >> N >> Q;
    vector<int> arr(N);
    for (int &x : arr) cin >> x;

    SegmentTree *ST = new SegmentTree(arr);
    ST->build(1, 0, N - 1);

    vector<int> qs;
    for (int q = 0; q < Q; ++q) {
        int ty, a, b;
        cin >> ty >> a >> b;
        if (ty == 1) {
            // ST.update(1, 0, N - 1, a, b); // 0-based
            ST->update(1, 0, N - 1, a - 1, b);  // 1-based
        } else {
            // int res = ST.update(1, 0, N - 1, a, b); // 0-based
            int res = ST->query(1, 0, N - 1, a - 1, b - 1);  // 1-based
            qs.push_back(res);
        }
    }

    for (int x : qs) cout << x << '\n';
}