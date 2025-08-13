/* 777 */
#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;

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
        this->table.resize(n, vector<int>(this->L, INF));
    }

    void build(vector<int>& arr) {
        for (int i = 0; i < N; ++i) {
            table[i][0] = arr[i];
        }
        for (int k = 1; k < L; ++k) {
            for (int i = 0; i < N - (1 << k) + 1; ++i) {
                table[i][k] =
                    min(table[i][k - 1], table[i + (1 << (k - 1))][k - 1]);
            }
        }
    }

    int query(int l, int r) {
        int k = highestPowerOf2(r - l + 1);
        return min(table[l][k], table[r - (1 << k) + 1][k]);
    }
};

void solve() {
    int N, Q;
    cin >> N >> Q;
    vector<int> arr(N);
    for (int& x : arr) cin >> x;
    SparseTable* st = new SparseTable(N);
    st->build(arr);
    vector<int> qs(Q);
    for (int q = 0; q < Q; ++q) {
        int l, r;
        cin >> l >> r;
        // int res = st.query(l, r); // 1 - based
        int res = st->query(l - 1, r - 1);  // 0-based
        qs[q] = res;
    }
    for (int q : qs) cout << q << '\n';
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int T = 1;
    // cin >> T;
    while (T--) solve();
}