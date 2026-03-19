# TAB — Common Keyboard Layout Size

This project solves a basic number theory problem: finding the minimum total size that can be constructed by two different linear forms.

The solution is based on linear Diophantine equations and the extended Euclidean algorithm.

---

## Problem

Given two constructions:

    k = s1 + c1 * x
    k = s2 + c2 * y

find the smallest integer `k` such that:

- `k ≥ n`
- `x, y ≥ 0`
- both constructions produce the same `k`

If no such `k` exists, output `IMPAWSIBLE`.

---

## Key Idea

Transform the condition into a linear Diophantine equation:

    c1 * x - c2 * y = s2 - s1

- A solution exists iff:

      gcd(c1, c2) | (s2 - s1)

- All feasible values of `k` form an arithmetic progression with step:

      lcm(c1, c2)

- Compute one feasible solution, then lift it to the smallest `k ≥ max(s1, s2, n)`

---

## Repository Structure

- `src/` — implementation  
- `tests/` — sample inputs and test scripts  
- `problem/` — original problem statement  

---

## Usage

### Build

See `src/README.md`

### Test

See `tests/README.md`

---

## Notes

- Handle edge cases when `c1 = 0` or `c2 = 0`
- Be careful with negative modulo in C
- This is a reference-level problem focused on applying standard number theory techniques
