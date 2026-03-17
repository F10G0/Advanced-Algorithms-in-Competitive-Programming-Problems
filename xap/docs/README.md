# Algorithm Notes

This directory contains materials explaining the core algorithm behind the XAP solution.

---

## Goal

Given a set of radar disks and a horizontal corridor

    100 < y < w + 100

the task is to remove the minimum number of radars such that a path exists
from the left boundary `x = 0` to the right boundary `x = l`
without intersecting any radar disk.

---

## Main Observation

The problem is not about explicitly constructing a path,
but about determining when a path is impossible.

A path does **not** exist if and only if:

> The radar disks form a continuous blocking chain connecting  
> the lower boundary `y = 100` and the upper boundary `y = w + 100`.

Therefore, the problem reduces to:

> Remove the minimum number of radars so that no such blocking chain exists.

---

## Geometric Reformulation

Each radar is classified as follows:

- **Mandatory removals**  
  Radars that already touch either boundary

- **Lower group**  
  Radars strictly below the corridor

- **Upper group**  
  Radars strictly above the corridor

After removing boundary-touching radars:

- lower radars connect only to the lower side
- upper radars connect only to the upper side

A blocking chain can then exist only if there is a connection
between a lower radar and an upper radar.

---

## Graph Interpretation

We construct a bipartite graph:

- Left side  (U): upper radars
- Right side (V): lower radars
- Edge (u, v): disks intersect (or touch)

Then:

> A blocking chain exists if and only if there is a connection  
> between the two sides in this graph.

---

## Core Insight

To eliminate all blocking chains, every upper–lower intersection must be broken.

This is equivalent to:

> Selecting a minimum set of vertices such that every edge is covered.

This is exactly the **minimum vertex cover** problem on a bipartite graph.

---

## Key Theorem

By Kőnig’s theorem:

    minimum vertex cover = maximum matching

So the problem can be solved by:

1. computing a maximum matching
2. recovering the corresponding minimum vertex cover

---

## Final Algorithm

1. Classify all radars  
   - remove those touching corridor boundaries

2. Build bipartite graph  
   - edges = upper–lower intersections

3. Compute maximum matching  
   - DFS-based augmenting paths

4. Recover minimum vertex cover  
   - alternating path traversal

5. Output result  
   - mandatory removals  
   - vertex cover nodes

---

## Implementation Notes

- Intersection condition:

      (x1 - x2)^2 + (y1 - y2)^2 <= (r1 + r2)^2

- Horizontal pruning is applied before full distance computation

- Adjacency list is stored in compressed form  
  (delta-linked edges, 4 bytes per edge)

---

## Files

- `xap_idea.md`  
  Core modeling and reduction (geometry → graph → matching)

- `xap_annotated_en.c`  
  English annotated implementation

- `xap_annotated_zh.c`  
  Chinese annotated implementation

- `algorithm.pdf`  
  Detailed technical write-up

- `templates/max_match.cpp`  
  Reference implementation of bipartite matching (algorithm template)

---

## Relationship to the Final Implementation

The final executable version is located in:

    ../src/xap.c

The files in this directory document the reasoning process
from geometric blocking conditions to the bipartite matching formulation
and its optimized implementation.
