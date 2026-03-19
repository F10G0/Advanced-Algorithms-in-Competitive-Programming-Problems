# TUV — Toy Value Unknown

This project implements an efficient algorithm to compute how many toys of size exactly `k` are detected by a camera in a tree-structured tunnel system.

The solution is based on reducing the splitting process to a path multiplicative factor and transforming the problem into interval counting.

---

## Problem

Given a tree of chambers connected by tunnels, toys are inserted from leaf chambers and repeatedly split across outgoing tunnels as they move through the system.

A camera is placed on a fixed tunnel and only detects piles of size exactly `k`.

The task is to determine the total number of toys detected across all given initial pile sizes.

---

## Key Idea

Instead of simulating the splitting process directly, we:

- cut the tree at the camera edge into two rooted subtrees
- map each leaf-to-camera path to a multiplicative factor
- reduce the process to checking whether a pile lies in an interval

For a path with factor `f`, a pile `m` contributes if:

- `f * k <= m < f * (k + 1)`

Thus, the problem becomes:

> for each leaf, count how many piles fall into its corresponding interval

---

## Repository Structure

- `src/` — final implementation  
- `docs/` — algorithm explanation and annotated code  
- `tests/` — sample inputs and automated test scripts  
- `problem/` — original problem statement  

---

## Usage

### Build

See `src/README.md`

### Test

See `tests/README.md`

---

## Highlights

- transforms a dynamic splitting process into static interval counting
- tree traversal with multiplicative path factors
- efficient binary search over sorted input
- optional optimization via factor grouping
- clean separation of main solution and alternative implementation

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This problem demonstrates how a seemingly complex simulation can be reduced to a simple mathematical condition by exploiting tree structure and path invariants.
