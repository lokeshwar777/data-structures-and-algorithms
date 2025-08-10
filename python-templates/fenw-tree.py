""" 777 """

# i=index, B=tree, d=delta, N=TREE_SIZE, B1=difference_array, B2=correction_array
# l=left, r=right,
# _u=point_update, _q=query, update=range_update, query=range_query, p_sum=prefix_sum


class FenwickTree:
    def __init__(self, N):
        self.N = N + 2
        self.B1 = [0] * self.N
        self.B2 = [0] * self.N

    def _q(self, B, i):
        s = 0
        while i > 0:
            s += B[i]
            i -= i & -i
        return s

    def _u(self, B, i, d):
        while i < self.N:
            B[i] += d
            i += i & -i

    def update(self, l, r, d):
        self._u(self.B1, l, d)
        self._u(self.B1, r + 1, -d)
        self._u(self.B2, l, -d * (l - 1))
        self._u(self.B2, r + 1, d * r)

    def p_sum(self, i):
        return self._q(self.B1, i) * i + self._q(self.B2, i)

    def query(self, l, r):
        return self.p_sum(r) - self.p_sum(l - 1)


N, Q = map(int, input().split())
FT = FenwickTree(N)
qs = []
for _ in range(Q):
    inp = list(map(int, input().split()))
    if inp[0]:
        _, l, r = inp
        q = FT.query(l, r)
        qs.append(q)
    else:
        _, l, r, d = inp
        FT.update(l, r, d)

for q in qs:
    print(q)
