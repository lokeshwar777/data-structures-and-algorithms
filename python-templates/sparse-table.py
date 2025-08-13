""" 777 """

INF = float('inf')


class SparseTable:
    def __init__(self, N):
        self.N = N
        self.L = N.bit_length()
        self.table = [[INF] * self.L for _ in range(self.N)]

    def build(self, arr):
        for i in range(self.N):
            self.table[i][0] = arr[i]
        for k in range(1, self.L):
            for i in range(self.N - (1 << k) + 1):
                self.table[i][k] = min(self.table[i][k - 1], self.table[i + (1 << (k - 1))][k - 1])

    def query(self, l, r):
        k = (r - l + 1).bit_length() - 1
        x = 1 << k
        return min(self.table[l][k], self.table[r - x + 1][k])


N, Q = [int(x) for x in input().split()]
arr = [int(x) for x in input().split()]
st = SparseTable(N)
st.build(arr)
qs = []
for _ in range(Q):
    l, r = [int(x) for x in input().split()]
    # result = st.query(l, r) # 1-based
    res = st.query(l - 1, r - 1)  # 0-based
    qs.append(res)

for res in qs:
    print(res)
