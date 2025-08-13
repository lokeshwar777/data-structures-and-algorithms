/* 777 */
#include <bits/stdc++.h>
using namespace std;
#define int long long

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

void solve() {
    int N, Q;
    cin >> N >> Q;
    vector<int> arr(N);
    for (int& x : arr) cin >> x;

    FenwickTree* FT = new FenwickTree(N);

    for (int i = 1; i <= N; ++i) FT->update(i, i, arr[i - 1]);

    vector<int> qs;
    for (int i = 0; i < Q; ++i) {
        int ty, a, b;
        cin >> ty >> a >> b;
        if (ty == 1) {
            // FT->update(a + 1, a + 1, b - arr[a - 1]);  // 0-based
            FT->update(a, a, b - arr[a - 1]);  // 1-based
            arr[a - 1] = b;
        } else {
            // int res = FT->query(a + 1, b + 1);  // 0-based
            int res = FT->query(a, b);  // 1-based
            qs.push_back(res);
        }
    }

    delete FT;
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