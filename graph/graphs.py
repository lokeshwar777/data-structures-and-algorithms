'''
1. use the set, only if it is necessary as it consumes more space
visited = set()
visited.add((x, y))

TODO : dijkstra using i,j and k
'''

# 1. ADJACENCY LISt

## 1.1 WEIGHTED
from collections import defaultdict, deque
def get_graph(edges): # returns adjacency dict or map
    graph=defaultdict(list)
    for u,v,w in edges:
        graph[u].append((v,w))
        # graph[v].append((u,w))
    return graph

def get_graph(n,edges): # returns adjacency list
    graph=[[] for _ in range(n)]
    for u,v,w in edges:
        graph[u].append((v,w))
        # graph[v].append((u,w))
    return graph

## 1.2 UNWEIGHTED
def get_graph(edges): # returns adjacency dict or map
    graph=defaultdict(list)
    for u,v in edges:
        graph[u].append(v)
        graph[v].append(u)
    return graph

def get_graph(n,edges): # returns adjacency list
    graph=[[] for _ in range(n)]
    for u,v in edges:
        graph[u].append(v)
        graph[v].append(u)
    return graph



# 2. BASIC TRAVERSAL

## 2.1 DFS

### 2.1.1 RECURSIVE
# vis=[False]*n
def dfs(node):
    vis[node]=True
    for neigh in graph[node]:
        if vis[neigh]:continue
        dfs(neigh)

### 2.1.2 ITERATIVE
def dfs(start):
    vis=[False]*n
    stack=[start]
    while stack:
        node=stack.pop()
        if vis[node]:continue
        vis[node]=True
        for neigh,cost in graph[node][::-1]:
            if vis[neigh]:continue
            stack.append(neigh)

## 2.2 BFS

### 2.2.1 ITERATIVE
def bfs(start,graph):
    vis=[False]*len(graph)
    queue=deque([start])
    vis[start]=True
    while queue:
        n=len(queue)
        for _ in range(n):
            node=queue.popleft()
            for neigh,cost in graph[node]:
                if vis[neigh]:continue
                vis[neigh]=True
                queue.append(neigh)

# COMPONENT

def getConnectedComponent(start,graph,vis):
        connected_component=[]
        def dfs(node):
            vis[node]=True
            connected_component.append(node)
            for neigh in graph[node]:
                if vis[neigh]:continue
                dfs(neigh)
        dfs(start)
        return connected_component
 
# 3. CYCLES

## 3.1 UNDIRECTED

##  3.1.1 DFS
# if you do dfs at that index it will tell whether you will get stuck in a cycle or not 
def detectCycle(n,graph): # returns the existence of cycle
    vis=[False]*n
    def dfs(node,par):
        vis[node]=True
        hasCycle=False
        for neigh in graph[node]:
            if vis[neigh] and neigh==par:continue
            if vis[neigh]:return True
            hasCycle|=dfs(neigh,node)
        return hasCycle
    for i in range(n):
        if vis[i]:continue
        if dfs(i,-1):return True
    return False

def getCycleLength(n,graph): # returns len of cycle if present else 0

def isBipartite(n,graph):  
    colors=[-1]*n
    def dfs(node,color=0):
        if colors[node]!=-1:return False
        colors[node]=color
        return any(colors[neigh]==color or dfs(neigh,color^1) for neigh in graph[node])
    return not any(dfs(i) for i in range(n))

def getCycleNodes(): # returns a list of nodes in the cycle


## 3.1.2 BFS

## 3.2 DIRECTED

##  3.2.1 DFS (TODO: try if isCycleNode[node]!=1:return isCycleNode[node])
def detect_cycle_directed(graph):
    n=len(graph)
    # -1->not vis, 0->vis and no cycle, 1->cycle and in recursion stack
    isCycleNode=[-1]*n
    
    def dfs(node):
        if isCycleNode[node]==1:return True
        if isCycleNode[node]==0:return False
        isCycleNode[node]=1
        for neigh in graph[node]:
            if neigh==node or dfs(neigh):return True
        isCycleNode[node]=0
        return False
        
    for i in range(n):
        if isCycleNode[i]==-1:dfs(i)
    return isCycleNode

## 3.2.2 BFS


# 4. SHORTEST PATH ALGOS

## 4.1 DIJKSTRA

### 4.1.1 GRID
from heapq import heappush
from heapq import heappop
from collections import defaultdict
def dijkstra_on_grid(grid): # dijkstra using heap or priority queue
    m, n = len(grid), len(grid[0])
    heap = [(0, 0, 0)]
    visited = [[False]*n for _ in range(m)]
    while heap:
        cost, x, y = heappop(heap)
        if x == m - 1 and y == n - 1:
            return cost
        if x < 0 or y < 0 or x >= m or y >=n or visited[x][y]:
            continue
        visited[x][y] = True
        directions = ((0, 1), (0, -1), (1, 0), (-1, 0))
        for i in range(4):
            dx, dy = directions[i]
            heappush(heap, (cost + (grid[x][y] != i + 1), x + dx, y + dy))
    
    return m + n - 1
# dijkstra_on_grid(grid)

### 4.1.2 UNDIRECTED GRAPH
def dijkstra_on_graph(n,edges):
    graph=defaultdict(list)
    for u,v,w in edges:
        graph[u].append((v,w))
        graph[v].append((u,w))
    
    def dijkstra(src): # returns sssp distance list from source to every node
        inf=10**10
        vis=[False]*n
        dist=[inf]*n
        heap=[(0,src)]
        while heap:
            cost, node = heappop(heap)
            if vis[node]:continue
            vis[node]=True
            dist[node]=cost
            for v,w in graph[node]:
                if vis[v]:continue
                heappush(heap,(cost+w,v))
        return dist
    
    src_to_node=dijkstra(0)
    dest_to_node=dijkstra(n-1)
# dijkstra_on_graph(n,edges):





