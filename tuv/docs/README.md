# Algorithm Notes

This directory contains materials explaining the core algorithm behind the TUV solution.

## Goal

Given a tree-shaped tunnel system and a set of initial toy piles, the task is to determine how many toys of size exactly `k` are detected by a camera placed on a specific edge.

Each experiment inserts a pile into a leaf chamber. As the pile moves through the tree, it is repeatedly split across available outgoing tunnels. Any remainder is discarded.

The camera only detects piles of size exactly `k` passing through its edge.

---

## Main Observation

The camera lies on a single edge. Removing this edge splits the tree into two independent rooted subtrees.

Any valid entrance that can send toys through the camera must be a leaf in one of these two subtrees.

Therefore, the problem reduces to analyzing all leaf-to-root paths in the two trees formed by cutting the camera edge.

---

## Path Factor Reduction

Consider a leaf and the unique path from that leaf to the camera edge.

At each chamber along the path, suppose the number of available forward tunnels is:

- d1, d2, ..., dt

Each step divides the pile by the current branching factor, discarding remainders.

Thus, a pile of size m arriving at the camera becomes:

floor(m / (d1 * d2 * ... * dt))

Define:

factor = d1 * d2 * ... * dt

Then the camera detects this pile if and only if:

floor(m / factor) = k

which is equivalent to:

factor * k <= m < factor * (k + 1)

---

## Interval Counting

For each leaf, all valid piles lie in the interval:

[factor * k, factor * (k + 1))

Thus, each leaf contributes:

(number of piles in interval) * k

After sorting all pile sizes, this can be computed using binary search.

---

## Tree Processing

### Cutting the Camera Edge

The camera edge is excluded from the adjacency structure, effectively splitting the tree into two rooted components.

We run the algorithm independently on both sides.

---

### Branch Count Interpretation

For each node:

childCount[x] = number of forward branches (excluding parent)

- childCount[x] == 0 → leaf (valid entrance)
- otherwise → contributes multiplicatively to the path factor

---

### DFS Accumulation

Starting from both camera-side roots:

- maintain a running factor
- multiply by childCount[x] when descending
- when reaching a leaf, compute its contribution via binary search

---

## Pruning

Let M be the maximum pile size.

If at any point:

factor * k > M

then no deeper node in that subtree can contribute.

Thus, the DFS branch can be pruned early.

---

## Alternative Implementation

- tuv_factor_grouping.c  
  An alternative implementation that first collects all leaf factors, sorts them, and groups identical values to perform batch counting.

This reduces repeated binary searches when many leaves share the same factor.

---

## Files

- tuv_annotated_en.c — English annotated implementation  
- tuv_annotated_zh.c — Chinese annotated implementation  
- tuv_factor_grouping.c — factor grouping variant  
- algorithm.pdf — detailed technical write-up  

---

## Relationship to the Final Implementation

The final executable version is located in ../src/tuv.c.

The files in this directory document the core idea, annotated implementation, and an alternative optimization strategy based on factor grouping.
