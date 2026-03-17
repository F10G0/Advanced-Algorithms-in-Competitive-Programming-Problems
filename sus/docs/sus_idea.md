# SUS – Idea & Solution

## Problem Modeling

We are given a weighted undirected graph with:
- n nodes
- m edges
- k key nodes
- q queries

Each query starts from a node s, and we must propagate a signal to all key nodes with minimal total cost.

### Propagation Rules

- Non-key nodes can forward the signal to at most one neighbor.
- Key nodes can forward the signal to any number of neighbors.

---

## Key Insight

The propagation process naturally splits into two phases:

### 1. Access Phase (Query → First Key Node)

Since non-key nodes cannot branch, the signal must travel along a single path to reach the first key node.

Therefore:

access_cost(s) = min distance from s to any key node

---

### 2. Propagation Phase (Among Key Nodes)

Once the signal reaches a key node, branching is allowed.

This reduces to:

Connect all key nodes with minimum total cost.

This is exactly a Minimum Spanning Tree (MST) problem.

---

## Graph Transformation

The original graph may not directly connect key nodes.

We construct a complete graph on key nodes:

weight(i, j) = shortest path distance between key_i and key_j

These distances are computed using Dijkstra.

---

## Final Formula

For any query node s:

answer(s) = access_cost(s) + mst_cost

---

## Algorithm

1. For each key node:
   - Run Dijkstra
   - Record distances to all other key nodes
   - Build a k × k distance matrix

2. Run Prim's algorithm:
   - Compute MST cost on the key-node graph

3. Run multi-source Dijkstra:
   - All key nodes as sources
   - Compute distance to nearest key node for every node

4. For each query:
   - Output: dist[s] + mst_cost

---

## Complexity

- k × Dijkstra: O(k · m log n)
- MST (Prim): O(k² log k)
- Multi-source Dijkstra: O(m log n)

---

## Why This Works

- Non-key nodes restrict propagation to a single path → shortest path
- Key nodes allow branching → tree structure
- Optimal broadcast structure among key nodes is an MST

---

## Key Takeaways

- Transform propagation rules into graph structure
- Separate local cost (access) and global cost (MST)
- Use shortest paths to build a metric closure
