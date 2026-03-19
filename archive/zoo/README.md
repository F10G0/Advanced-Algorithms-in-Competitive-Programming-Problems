# ZOO — Minimum Initial Energy with Limited Rests

This project implements an efficient algorithm to determine the minimum initial energy required to complete a sequence of jumps under limited rest operations.

The solution is based on binary search over the answer combined with a greedy feasibility check.

---

## Problem

Given a sequence of energy costs `e_1, e_2, ..., e_n`, you must process them in order.

- You start with initial energy `x`
- You may rest at most `k` times
- After the `t`-th rest, your energy is restored to:

    x - t

Determine the minimum initial energy `x` such that all jumps can be completed.

---

## Key Idea

Instead of simulating all possible strategies, we:

- binary search the initial energy `x`
- interpret the process as splitting the sequence into at most `k+1` segments
- assign segment capacities:

    x, x-1, x-2, ..., x-k

- greedily consume as many consecutive jumps as possible in each segment

If all jumps are covered, the value is feasible.

---

## Repository Structure

- `src/` — final implementation  
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

- binary search on answer with monotonic feasibility
- greedy segmentation with strictly decreasing capacities
- linear-time feasibility check
- clean and minimal implementation

---

## Notes

This problem demonstrates a standard pattern of combining binary search with greedy verification.
The core insight is that later segments always have weaker capacity, making greedy assignment optimal.
