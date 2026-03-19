# PSS — Maximum Enjoyment with Distinct Scents

This project implements an efficient algorithm to compute the maximum total weight of **distinct elements** in a contiguous subarray.

The solution is based on a right-endpoint sweep combined with interval contribution transfer and a lazy segment tree.

---

## Problem

Given a sequence of scents `f[1..n]`, where each scent `j` has a weight `w[j]`, determine the maximum value of:

    sum of w[j] over all distinct scents j appearing in [L, R]

for any contiguous interval `[L, R]`.

Each scent contributes at most once within the interval, regardless of how many times it appears.

---

## Key Idea

Instead of enumerating all intervals, we:

- fix the right endpoint `R = i`
- maintain the value of all intervals `[L, i]`
- update contributions using the last and second-last occurrences of each scent

For each scent, its contribution forms an interval on the left-endpoint axis.
When a new occurrence appears, this contribution interval shifts:

- remove contribution from `[l2+1, l1]`
- add contribution to `[l1+1, i]`

All interval values are maintained using a segment tree, and the maximum is tracked at each step.

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

- transforms a 2D interval problem into a 1D sweep over right endpoints
- models distinct-element contribution as shifting intervals
- efficient range updates via lazy segment tree
- handles large constraints up to $10^6$ elements
- fully reproducible with automated tests

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/pss_idea.md` — core modeling insight  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This problem demonstrates a classic advanced technique:

> maintaining all possible left endpoints while sweeping the right endpoint.

It is a representative example of combining occurrence tracking with range data structures to handle distinct-element constraints efficiently.
