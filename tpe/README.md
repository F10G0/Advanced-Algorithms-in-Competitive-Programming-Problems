# TPE — Shortest Period of Substrings

This project implements an efficient algorithm to compute the shortest period length of substrings under a large number of queries.

The solution is based on divisor enumeration combined with rolling hash.

---

## Problem

Given a string `s`, each query specifies a substring `s[l..r]`.

For each query, determine the length of the shortest period of the substring.

---

## Key Idea

Instead of checking all possible period lengths, we:

- reduce candidates to divisors of the substring length
- reformulate periodicity as shifted substring equality
- use rolling hash for constant-time comparison

The first valid divisor is the shortest period.

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

- reduces string periodicity to divisor enumeration
- `O(1)` substring comparison via rolling hash
- `O(n log n)` preprocessing for all divisor lists
- handles millions of queries efficiently
- clear separation of idea, annotated code, and final solution

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/tpe_idea.md` — detailed problem reformulation  
- `docs/algorithm.pdf` — technical write-up  

---

## Notes

This project focuses on transforming a string problem into a number-theoretic formulation, enabling efficient large-scale query processing.
