# RAI — Rainy Days

This project implements an efficient algorithm to maximize the total value collected over time-dependent rewards.

The solution is based on ordering linear functions and maintaining a prefix-optimal sequence.

---

## Problem

Given $n$ glades, each with initial value $b_i$ and daily growth $a_i$, determine an ordering of visits such that for every $k$, the total collected value in $k$ days is maximized.

---

## Key Idea

Instead of making greedy choices per day, we:

- model each glade as a function $f(t) = a \cdot t + b$
- construct a prefix-optimal ordering via insertion
- maintain the sequence using a splay tree with lazy propagation

This ensures that all prefix sums are optimal.

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

- reduction to linear function ordering
- prefix-optimal sequence construction
- splay tree with lazy propagation for efficient updates
- separation of naive prototype and optimized implementation
- fully reproducible with automated tests

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This project focuses on clarity of algorithmic design and implementation, rather than extensive engineering complexity.
