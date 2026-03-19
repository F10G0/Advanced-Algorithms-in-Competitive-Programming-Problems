# SLI — Sliding Cat Puzzle

This project implements an efficient algorithm to compute the minimum total effort required to visit all slots in the Sliding Cat Puzzle.

The solution is based on greedy direction selection with a single optimal correction and interval-based path reconstruction.

---

## Problem

Given `n` slots arranged in a row and a starting position `s`, perform `n - 1` moves to visit all slots.

For each move `i`:

- moving left costs `l[i]`
- moving right costs `r[i]`

A move can jump across multiple slots, so the cost depends only on the chosen direction.

The goal is to:

- minimize the total cost, and  
- output a valid visiting order achieving this minimum.

---

## Key Idea

Instead of reasoning about movement distances, we observe that:

- the visited set always forms a contiguous interval
- each move only decides whether to expand left or right

We therefore:

- choose the cheaper direction greedily for each move
- check whether the resulting sequence can reach both sides of the starting position
- if necessary, flip exactly one move with minimum additional cost

The final visiting order is reconstructed using interval boundaries and direction segments.

---

## Repository Structure

- `src/` — final implementation  
- `docs/` — algorithm explanation, annotated code, and alternative version  
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

- linear-time solution (`O(n)`)
- greedy optimization with a single optimal correction
- clean separation between cost computation and path reconstruction
- segment-based construction of the visiting order
- multiple implementations for comparison (final + alternative)
- fully reproducible with automated tests

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This problem reduces a seemingly complex movement process to a simple directional decision sequence.

The key insight is that feasibility can be restored with at most one correction, leading to a clean and efficient solution.
