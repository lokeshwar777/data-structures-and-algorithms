"""
---------------------------------------------------
| Indexing | parent at | left child | right child |
---------------------------------------------------
|  0-based |     n     |   2n + 1   |   2n + 2    | <- iterative seg tree
|  1-based |     n     |     2n     |   2n + 1    | <- recursive seg tree
---------------------------------------------------
rest of the other formulas/relationships are shifted variants
"""


# GPT
"""
node  : Current Tree Node (index in self.tree[]) (1-based)
start : Current Segment Start (index in nums[]) (0-based)
end   : Current Segment End (index in nums[]) (0-based)
index : Update Index (position in nums[] to update) (0-based)
value : Update Value (new value to set at index)
l     : Query Left Bound (inclusive) (0-based)
r     : Query Right Bound (inclusive) (0-based)

start, end → "Where am I in the tree?"
l, r → "What range am I being asked about?"
index, value → "What exactly do I want to change?"
"""


# default -> nums (0-based), tree[] (1-based)
class SegTree:
    # root = 0  # left=(n<<1)+1, right=(n<<1)+2
    root = 1  # left=n<<1, right=(n<<1)+1

    """
    Initilises the tree with initial value for each node and
    updates default merge with custom merge logic if any
    """

    def __init__(self, nums, init_val, merge=None) -> None:
        self.nums = nums
        # 1 + 2 + 4 + ... + 2^(log2(n)) < 2^(log2(n) + 1) < 4n
        nodes = 4 * len(nums)
        # separate instances for non-primitive DS
        self.tree = [init_val for _ in range(nodes)]
        self.merge = merge if merge else lambda a, b: a + b

    """
    Builds the tree with nums as leaf nodes and
    merges result of subtrees for intermediary nodes
    """

    def build(self, node, start, end) -> None:
        if start > end:  # outside segment
            return
        if end == start:  # leaf node found
            self.tree[node] = self.nums[start]
            return
        mid = (start + end) >> 1
        left_subtree_root = node << 1
        right_subtree_root = (node << 1) + 1
        self.build(left_subtree_root, start, mid)
        self.build(right_subtree_root, mid + 1, end)
        left_subtree = self.tree[left_subtree_root]
        right_subtree = self.tree[right_subtree_root]
        self.tree[node] = self.merge(left_subtree, right_subtree)

    """
    updates the corresponding leaf node assiciated with `index` with `value`
    propogates changes to parent nodes for tree updation
    """

    def update(self, node, start, end, index, value):
        if start > end:  # outside segment
            return
        if start == end:  # leaf node found
            self.tree[node] = value
            return
        mid = (start + end) >> 1
        left_subtree_root = node << 1
        right_subtree_root = (node << 1) + 1
        if index <= mid:
            self.update(left_subtree_root, start, mid, index, value)
        else:
            self.update(right_subtree_root, mid + 1, end, index, value)
        left_subtree = self.tree[left_subtree_root]
        right_subtree = self.tree[right_subtree_root]
        self.tree[node] = self.merge(left_subtree, right_subtree)

    """
    retrieves the merged result of the values in a specific range
    no update happens here, get result from subtrees, merge them and return
    """

    def query(self, node, start, end, l, r):
        if l > r:  # invalid segment
            return 0  # dummy/empty value
        if l == start and end == r:  # segments exact match with range
            return self.tree[node]
        mid = (start + end) >> 1
        left_subtree_root = node << 1
        right_subtree_root = (node << 1) + 1
        left_subtree = self.query(left_subtree_root, start, mid, l, min(mid, r))
        right_subtree = self.query(right_subtree_root, mid + 1, end, max(l, mid + 1), r)
        return self.merge(left_subtree, right_subtree)


"""
takes 2 nodes as input (left and right nodes)
returns result based on merging strategy
"""


def custom_merge(a, b):
    return a + b


N, q = map(int, input().split())
arr = list(map(int, input().split()))

seg_tree = SegTree(nums=arr, init_val=0, merge=custom_merge)
seg_tree.build(seg_tree.root, 0, N - 1)

results = []  # offline queries
for _ in range(q):
    # ty -> 1-update, 2-range query
    ty, a, b = map(int, input().split())

    if ty == 1:
        # seg_tree.update(node=seg_tree.root, start=0, end=N-1, index=a, value=b) # 0-based
        seg_tree.update(node=seg_tree.root, start=0, end=N-1, index=a-1, value=b)  # 1-based
    else:
        # ans = seg_tree.query(node=seg_tree.root, start=0, end=N-1, l=a, r=b) # 0-based
        ans = seg_tree.query(node=seg_tree.root, start=0, end=N-1, l=a-1, r=b-1)  # 1-based
        results.append(ans)
for result in results:
    print(result)
