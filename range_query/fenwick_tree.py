""" 777 """

# Fenwick Tree
"""
i   binary   last set bit   group covers
1   0001     1              [1]
2   0010     2              [1,2]
3   0011     1              [3]
4   0100     4              [1,2,3,4]
5   0101     1              [5]
6   0110     2              [5,6]
7   0111     1              [7]
8   1000     8              [1,2,3,4,5,6,7,8]

update - left to right / top to bottom <-
query - right to left / bottom to top -> 
"""

########################## POINT UPDATE RANGE QUERY ###########################


class FenwickTree:
    def __init__(self, N):
        self.TREE_SIZE = N + 2
        self.tree = [0] * self.TREE_SIZE

    def point_update(self, index, delta):
        while index < self.TREE_SIZE:
            self.tree[index] += delta
            index += index & -index

    def point_query(self, index):
        res = 0
        while index > 0:
            res += self.tree[index]
            index -= index & -index
        return res


###############################################################################


########################## POINT UPDATE RANGE QUERY ###########################
def lsb(x): return x & -x


class FenwickTree:  # 1-based
    def __init__(self, size):
        self.TREE_SIZE = size + 1
        self.tree = [0] * self.TREE_SIZE

    def update(self, index, delta):  # delta/diff prev value vs current
        while index < self.TREE_SIZE:
            self.tree[index] += delta
            index += lsb(index)

    def query(self, index):
        result = 0
        while index > 0:
            result += self.tree[index]
            index -= lsb(index)
        return result


N, Q = map(int, input().split())
arr = list(map(int, input().split()))
FT = FenwickTree(N)
for index, value in enumerate(arr):
    FT.update(index + 1, value)  # 1-based
queries = []
for _ in range(Q):
    ty, a, b = map(int, input().split())
    if ty == 1:
        FT.update(a, b - arr[a - 1])
        arr[a - 1] = b
    else:
        total = FT.query(b)
        not_needed = FT.query(a - 1)
        queries.append(total - not_needed)

for q in queries:
    print(q)


###############################################################################

# The story of optimising range (query + update) from O((r - l + 1) * logn) -> O(logn)
"""
1. if we use the tree to store prefix sums we can do
    - point update in O(logn)
    - range query in O(logn)
    - but range update (l, r) in O((r - l + 1) * logn) <-- slow / not efficient
2. whenever we need to calculate / change sum over a range -> diff array 
    - +x at l & -x at r+1
3. our tree still stores the prefix sums only but we need to slightly think differently
    - previously we did (sum(l -> r) = prefix(r) - prefix(l - 1)), prefix(i) = query(i)
      but now its going to be 
    - prefix(i) = query(i) * i <--- miscounting/overcounting/overestimation
    - so we need a separate correction factor kind of thing for each i
    - a separate array in simple words but technically its called a tree
    - finally prefix(i) = FT1.query(i) * i - intercept adjustment / offset from FT2
    - finally prefix(i) = FT1.query(i) * i - FT2.query(i)
4. FT1.update(i) -> update(l, +x), update(r + 1, -x)
    - starting from l add 𝜹 to the slope
    - starting from r subtract 𝜹 to the slope (stop slope)
5. FT2.update(i) -> update(l, -x * (l - 1)), update(r + 1, 𝜹 * r)
    - l -> constant before l and intercept correction starts 
    - r+1 -> function remains constant after r

!!!PROOF!!!
𝜹 means x
f(i) = 𝜹 * [0 (i<l), i-l+1 (i>=l), r-l+1 (i>r)]
f(i) = 𝜹 * [0 (i<l), min(i,r)-l+1 (i>=l)]
f(i) = 𝜹 * [max(0, min(i,r)-l+1)]
f(i) = 𝜹 * [min(i,r)-l+1, no updates before l so update at `l`]
f(i) = 𝜹 * [i-l+1 or r-l+1]
f(i) = 𝜹*i+𝜹*(1-l) or 𝜹*(r-l+1) = start_offset or end_offset
𝜹*(r-l+1) = 𝜹*r + -𝜹*(l-1) = constant -𝜹(l-1) => [𝜹*r is extra]
start_offset -> intercept at -𝜹(l-1) for correction
end_offset -> 𝜹*r is to make f(i) constant = -𝜹(l-1) from r+1 (i>r)
"""

########################## RANGE UPDATE RANGE QUERY ###########################

""" 777 """


class FenwickTree:
    def __init__(self, N):
        self.TREE_SIZE = N + 2
        # difference array/tree for deltas/increments
        self.diff_tree = [0] * self.TREE_SIZE
        # correction array/tree for intercept adjustment
        self.corr_tree = [0] * self.TREE_SIZE

    def point_update(self, tree, index, delta):
        while index < self.TREE_SIZE:
            tree[index] += delta
            index += index & -index

    def range_update(self, left, right, delta):
        self.point_update(self.diff_tree, left, delta)
        self.point_update(self.diff_tree, right + 1, -delta)
        self.point_update(self.corr_tree, left, -delta * (left - 1))
        self.point_update(self.corr_tree, right + 1, delta * right)

    def query(self, tree, index):
        res = 0
        while index > 0:
            res += tree[index]
            index -= index & -index
        return res

    def point_query(self, index):
        diff_sum = self.query(self.diff_tree, index)
        correction = self.query(self.corr_tree, index)
        return diff_sum * index + correction

    def range_query(self, left, right):
        right_prefix_sum = self.point_query(right)
        left_prefix_sum = self.point_query(left - 1)
        return right_prefix_sum - left_prefix_sum


N, Q = map(int, input().split())
FT = FenwickTree(N)
queries = []
for _ in range(Q):
    inp = list(map(int, input().split()))
    if inp[0]:
        _, left, right = inp
        query = FT.range_query(left, right)
        queries.append(query)
    else:
        _, left, right, delta = inp
        FT.range_update(left, right, delta)

###############################################################################
