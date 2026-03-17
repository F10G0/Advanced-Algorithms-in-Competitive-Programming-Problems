# XAP – Solution Idea

## 1. Problem Reformulation

The pigeon flies inside the corridor

`100 < y < w + 100`

and radar disks are forbidden regions, including their boundaries. The pigeon may follow any polygonal path.

The key observation is that the problem is not to explicitly construct a path, but to determine when such a path exists.

A path exists if and only if the radar disks do **not** form a continuous barrier connecting:

- the lower boundary `y = 100`
- the upper boundary `y = w + 100`

---

## 2. Key Insight

A blocking configuration must contain a connected chain of radar disks that spans from the lower boundary to the upper boundary.

Once no such chain exists, the corridor is traversable, because the pigeon may use any polygonal path and can route around obstacles that do not separate the corridor from bottom to top.

---

## 3. Classification of Radars

Each radar belongs to one of the following categories.

### 3.1 Mandatory removals

A radar must be removed immediately if it already touches a corridor boundary:

- a lower radar whose disk reaches `y = w + 100`
- an upper radar whose disk reaches `y = 100`

These radars are unavoidable obstacles, because they already directly participate in any possible blocking structure.

### 3.2 Remaining radars

After removing the boundary-touching radars, every remaining radar belongs to exactly one of two groups:

- **Lower group:** radars below the corridor
- **Upper group:** radars above the corridor

At this point, no remaining radar touches the corridor boundaries.

---

## 4. Critical Observation

A blocking chain must connect

`lower boundary -> ... -> upper boundary`

After preprocessing:

- lower radars are the only candidates connected to the lower side
- upper radars are the only candidates connected to the upper side

Therefore, a blocking chain can exist only if some lower radar intersects some upper radar.

This is the reason the problem can be reduced to a bipartite graph.

---

## 5. Reduction to a Bipartite Graph

Construct a bipartite graph as follows:

- left side `U`: all upper radars
- right side `V`: all lower radars

Add an edge `(u, v)` if and only if the two corresponding radar disks intersect or touch.

Two disks intersect exactly when

`(x1 - x2)^2 + (y1 - y2)^2 <= (r1 + r2)^2`

Why this works:

- a chain that blocks the corridor must eventually connect the lower part to the upper part
- such a connection is represented by an upper-lower intersection
- intersections only within the same side do not by themselves bridge the corridor

So every relevant blocking structure is captured by edges of this bipartite graph.

---

## 6. Graph-Theoretic Reformulation

We now want to remove the minimum number of radars so that no upper-lower connection remains.

Equivalently, we want to choose a minimum set of vertices such that every edge has at least one endpoint in the chosen set.

That is exactly the **minimum vertex cover** problem on a bipartite graph.

So the original geometric problem becomes:

> Find a minimum vertex cover in the bipartite graph of upper-lower radar intersections.

The total answer is:

`mandatory removals + minimum vertex cover`

---

## 7. Key Theorem

By **Kőnig’s Theorem** for bipartite graphs:

`minimum vertex cover size = maximum matching size`

So instead of solving minimum vertex cover directly, we can:

1. compute a maximum matching
2. recover a minimum vertex cover from it

---

## 8. Algorithm Outline

### Step 1: Classify radars

For each radar:

- if it touches a corridor boundary, mark it for destruction
- otherwise place it into the lower group or upper group

### Step 2: Build the bipartite graph

For every upper radar `u` and lower radar `v`:

- first apply a cheap horizontal pruning condition
- then, if necessary, perform the exact disk-intersection test
- add an edge if the two disks intersect

### Step 3: Compute maximum matching

Use a DFS-based augmenting path algorithm on the bipartite graph.

### Step 4: Recover minimum vertex cover

Start from unmatched vertices on the left side and traverse alternating paths:

- non-matching edges from left to right
- matching edges from right to left

Then the standard formula gives:

`cover = (left - visited_left) union visited_right`

### Step 5: Output the final set of destroyed radars

The final destroyed set consists of:

- all mandatory removals from preprocessing
- all radars selected by the minimum vertex cover

---

## 9. Important Implementation Notes

### 9.1 Geometry pruning

Before computing the full squared distance, the implementation first checks whether the horizontal distance is small enough.

This avoids many unnecessary intersection tests.

### 9.2 Memory optimization

The implementation stores adjacency information in compressed form:

- edges use a compact structure
- adjacency traversal uses delta links
- this reduces memory usage significantly

This is important because the memory limit is tight.

---

## 10. Complexity

Let `n <= 5000` be the number of radars.

- classification: `O(n)`
- graph construction: up to `O(n^2)` in the worst case
- matching: standard DFS-based bipartite matching complexity

In practice, the geometric pruning improves performance substantially.

---

## 11. Summary

The core transformation is:

```text
geometric blocking problem
-> topological separation condition
-> bipartite graph on upper/lower intersections
-> minimum vertex cover
-> maximum matching
