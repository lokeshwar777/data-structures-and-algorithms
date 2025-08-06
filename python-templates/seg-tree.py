class SegTree:
    def __init__(self, arr) -> None:
        self.arr = arr
        self.t = [0 for _ in range(4 * len(arr))]

    def merge(self, x, y):
        return x + y

    def build(self, v, tl, tr) -> None:
        if tr == tl:
            self.t[v] = self.arr[tl]
            return
        tm = (tl + tr) // 2
        self.build(2 * v, tl, tm)
        self.build(2 * v + 1, tm + 1, tr)
        self.t[v] = self.merge(self.t[2 * v], self.t[2 * v + 1])

    def update(self, v, tl, tr, i, x):
        if tl == tr:
            self.t[v] = x
            return
        tm = (tl + tr) >> 1
        if i <= tm:
            self.update(2 * v, tl, tm, i, x)
        else:
            self.update(2 * v + 1, tm + 1, tr, i, x)
        self.t[v] = self.merge(self.t[2 * v], self.t[2 * v + 1])

    def query(self, v, tl, tr, l, r):
        if l > r:
            return 0
        if l == tl and tr == r:
            return self.t[v]
        tm = (tl + tr) >> 1
        return self.merge(
            self.query(2 * v, tl, tm, l, min(tm, r)),
            self.query(2 * v + 1, tm + 1, tr, max(l, tm + 1), r)
        )


N, q = map(int, input().split())
arr = list(map(int, input().split()))

sgt = SegTree(arr)
sgt.build(1, 0, N - 1)

results = []
for _ in range(q):
    ty, a, b = map(int, input().split())

    if ty == 1:  # v, tl, tr, i, x
        sgt.update(1, 0, N - 1, a, b)  # 0-based
        # sgt.update(1, 0, N - 1, a - 1, b)  # 1-based
    else:
        ans = sgt.query(1, 0, N - 1, a, b)  # 0-based
        # ans = sgt.query(1, 0, N - 1, a - 1, b - 1)  # 1-based
        results.append(ans)

for result in results:
    print(result)
