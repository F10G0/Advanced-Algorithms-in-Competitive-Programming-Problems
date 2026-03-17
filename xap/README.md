# XAP — Blocking Radar Chains via Bipartite Matching

This project implements an efficient algorithm to determine the minimum number of radars that must be removed so that a safe path exists across a corridor.

The solution is based on geometric reformulation and reduction to bipartite matching.

---

## Problem

Given a set of circular radar regions and a horizontal corridor

    100 < y < w + 100

determine the minimum number of radars to remove such that there exists a path
from the left boundary `x = 0` to the right boundary `x = l`
that does not intersect any radar disk.

---

## Key Idea

Instead of searching for a path directly, we observe:

- a path is impossible if and only if radars form a blocking chain  
  connecting the lower boundary to the upper boundary

We then:

- classify radars into upper and lower groups
- build a bipartite graph based on intersections
- reduce the problem to minimum vertex cover
- solve it via maximum matching (Kőnig’s theorem)

---

## Repository Structure

- `src/`  
  final implementation (`xap.c`)

- `docs/`  
  algorithm explanation, annotated code, and technical write-up

- `tests/`  
  sample inputs, outputs, and automated test scripts

- `problem/`  
  original problem statement

---

## Usage

### Build

See:

    src/README.md

### Test

See:

    tests/README.md

---

## Highlights

- geometric → graph reduction (non-trivial modeling)
- bipartite matching + minimum vertex cover
- memory-optimized adjacency structure (delta-linked edges)
- efficient pruning for intersection checks
- clean separation of idea, annotated code, and final implementation

---

## Documentation

- `docs/README.md`  
  concise algorithm overview

- `docs/xap_idea.md`  
  full modeling and reduction

- `docs/algorithm.pdf`  
  detailed technical write-up

---

## Notes

This project emphasizes algorithmic modeling and transformation:

a geometric path-existence problem is converted into a classical graph problem,
then solved using well-established combinatorial techniques.
