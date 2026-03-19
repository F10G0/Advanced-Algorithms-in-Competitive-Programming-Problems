# PPM — Puzzle of the Perfect Meows

This project implements an efficient algorithm to construct a minimum-cost prefix-free set of strings.

The solution is based on a tree modeling of prefix constraints and a greedy expand-and-replace strategy.

---

## Problem

Given `k` letters with positive costs `w[i]`, construct exactly `n` distinct strings such that:

- no string is a prefix of another  
- the total cost is minimized  

The cost of a string is the sum of its character costs.

---

## Key Idea

Instead of working directly with strings, we model the problem as a leaf-selection problem in a weighted infinite `k`-ary tree.

- each string corresponds to a root-to-node path  
- prefix-free constraint ⇒ all chosen strings must be leaves  

The algorithm proceeds in two stages:

- generate a set of cheap candidate leaves by expanding the smallest leaf  
- iteratively improve the set by expanding the minimum leaf and replacing the largest leaves when beneficial  

The process stops when no further improvement is possible.

---

## Repository Structure

- `src/` — final implementation (min-max heap solution)  
- `docs/` — algorithm explanation and annotated code  
- `tests/` — sample inputs and automated test scripts  
- `problem/` — original problem statement  
- `versions/` — historical implementations (two-heap approaches)  

---

## Usage

### Build

See `src/README.md`

### Test

See `tests/README.md`

---

## Highlights

- non-trivial greedy strategy (expand + replace)  
- tree-based modeling of prefix-free constraints  
- efficient min-max heap implementation  
- clear evolution from two-heap solutions to final optimized version  
- fully reproducible with automated tests  

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This problem demonstrates how structural constraints (prefix-free sets) can be transformed into optimization over tree leaves, and how careful greedy design combined with appropriate data structures leads to an efficient solution.
