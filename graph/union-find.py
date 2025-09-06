class DisjointUnion:
    rank = []
    parent = []
    size = []

    def __init__(self, n):
        self.rank = [1]*n
        self.size = [1]*n
        self.parent = list(range(n+1))

    def find(self, node):
        if node == self.parent[node]:
            return node
        self.parent[node] = self.find(self.parent[node])  # path compression
        return self.parent[node]

    def unionByRank(self, u, v):
        ulp_u = self.find(u)
        ulp_v = self.find(v)
        if ulp_u == ulp_v:
            return
        if self.rank[ulp_u] < self.rank[ulp_v]:
            self.parent[ulp_u] = ulp_v
        elif self.rank[ulp_u] < self.rank[ulp_v]:
            self.parent[ulp_v] = ulp_u
        else:
            self.parent[ulp_u] = ulp_v
            self.rank[ulp_v] += 1

    def unionBySize(self, u, v):
        ulp_u = self.find(u)
        ulp_v = self.find(v)
        if ulp_u == ulp_v:
            return
        if self.size[ulp_u] < self.size[ulp_v]:
            self.parent[ulp_u] = ulp_v
            self.size[ulp_v] += self.size[ulp_u]
        elif self.size[ulp_u] < self.size[ulp_v]:
            self.parent[ulp_v] = ulp_u
            self.size[ulp_u] += self.size[ulp_v]
        else:
            self.parent[ulp_u] = ulp_v
            self.size[ulp_v] += self.size[ulp_u]

    def isConnected(self, a, b):
        return self.find(a) == self.find(b)


n, q = list(map(int, input().split()))
unionFind = DisjointUnion(n)
for _ in range(q):
    ty, u, v = list(map(int, input().split()))
    if ty:
        print(1 if unionFind.isConnected(u, v) else 0)
    else:
        unionFind.unionBySize(u, v)
    # else: unionFind.unionByRank(u,v)
