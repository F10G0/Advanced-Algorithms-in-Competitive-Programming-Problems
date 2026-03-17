# PHR — Minimum Denominator from Rounded Decimal

This project implements an efficient algorithm to recover the minimum possible denominator of a fraction that rounds to a given decimal value.

The solution is based on Stern–Brocot tree encoding and interval reconstruction.

---

## Problem

Given a decimal number in `[0, 1]` with fixed precision, determine the smallest denominator `q` such that there exists a fraction `p / q` which rounds to this value.

---

## Key Idea

Instead of searching over all fractions, we:

- construct the rounding interval `[l, r)`
- represent both boundaries using Stern–Brocot paths
- compute their longest common prefix (LCP)

The LCP corresponds to the simplest fraction inside the interval.

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

- integer-only implementation (no floating point errors)
- efficient path compression via Euclidean-style batching
- clean separation of prototype, annotated code, and final solution
- fully reproducible with automated tests

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This project focuses on clarity of algorithmic design and implementation, rather than extensive engineering complexity.
