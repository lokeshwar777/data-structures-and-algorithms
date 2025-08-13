""" 777 """

"""
Sparse table is a 2D array/matrix/grid
rows -> indices
cols -> powers (precomputed range for 2^power)

sparse_table[row][col] = sparse_table[index][power] 
    denotes the precomputed value of the range of elements
    starting at `index` with a length of 2^power

    what is the minimum of 2^powers elements starting at index?

Precomputation: O(N * logN)


Query: O(1)
    [left, right] = [L, R]

    indices -> [0, 1, 2, .., L, ..., ..., R] # 0-based array
                            | <-R - L + 1-> |
    let's say we need to find the minimum of the range (L, R) inclusive
    L .. R -> total R - L + 1 elements

    let's assume number, `window` := R - L + 1

    case 1:
        if `window` is a power of 2 then no problem
        we just double count the whole `window`
        result := min(L .. R, R .. L)

    case 2:
        if `window` is not a power of 2 the we find the
        highest power of 2 less than `window`, call it `LOG`
            how? leftmost setbit in `window`
            
        lets assume X: 2^LOG or 1 << LOG
        result := min(X elements starting at L, X elements ending at R)
        result := min(L ... (X elements), (X elements) ... R)
        result := min(L .. (L+X-1), (R-X+1) .. R)

    implementation for both the cases is same
    result := min(sparse_table[L][LOG], sparse_table[R-X+1][LOG])

    Note: for general purpose use a custom `combine()` instead of `min()`
"""


def highest_power_of_2(x: int):
    """highest power of 2 less than or equal to x"""
    return x.bit_length() - 1


INF = float('inf')
INITIAL_VALUE = INF  # INF for min, -INF for max, 1 for GCD


class SparseTable:
    def __init__(self, SIZE):
        self.SIZE = SIZE
        self.MAX_POWER = highest_power_of_2(SIZE) + 1
        self.sparse_table = [[INITIAL_VALUE] * self.MAX_POWER for _ in range(self.SIZE)]

    def combine(self, a, b):
        result = min(a, b)
        return result

    def build(self, arr):
        """what is the combine() of 2^power elements starting at and index"""
        # we take help of already computed values in each iteration
        # so we need a back pointer in this case is power - 1
        for index in range(self.SIZE - 1, -1, -1):
            self.sparse_table[index][0] = arr[index]

            for power in range(1, self.MAX_POWER):
                if index + (2 ** power) > self.SIZE:
                    break
                previous_power = power - 1
                self.sparse_table[index][power] = self.combine(
                    self.sparse_table[index][previous_power],
                    self.sparse_table[index + (2 ** previous_power)][previous_power]
                )

    def query(self, left, right):
        window = right - left + 1
        power = highest_power_of_2(window)
        return self.combine(self.sparse_table[left][power], self.sparse_table[right - (2 ** power) + 1][power])


N, Q = [int(x) for x in input().split()]
arr = [int(x) for x in input().split()]
st = SparseTable(N)
st.build(arr)
results = []
for _ in range(Q):
    l, r = [int(x) for x in input().split()]
    # result = st.query(l, r) # 1-based
    result = st.query(l - 1, r - 1)  # 0-based
    results.append(result)

for result in results:
    print(result)

"""
example:
^         0 1 2 4 8 16 ...
|    0 -> 3 2 2 1 1  1 ...
|    1 -> 2 2 1
|    2 -> 4 4 1
|    3 -> 5 1 1
|    4 -> 1 1 1
|    5 -> 1 1
|    6 -> 5 3
|    7 -> 3
X = pow of 2 - 1
min ([curindex][X], [curindex + X][X])
"""
