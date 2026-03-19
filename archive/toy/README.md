# TOY — Minimum Cost Toy Purchase

This project implements a simple and efficient algorithm to determine the minimum cost required to purchase a fixed number of toys under budget constraints.

The solution is based on feasibility filtering and selecting the optimal (lowest-price) valid option.

---

## Problem

Given:

- a required number of toys `n`
- a total budget `b`
- `s` dollar-stores
- each store has a fixed price `p_i`
- each store provides availability `a_{i,j}` for each of `d` days

You must:

- choose exactly one store and one day
- buy exactly `n` toys
- ensure total cost does not exceed the budget

Output the minimum total cost, or `GIFT MY SOUL` if no valid option exists.

---

## Key Idea

Instead of exploring combinations, observe that:

- the total cost is `n × p_i`
- for a store to be valid, it must have at least one day with `a_{i,j} ≥ n`

Thus, we:

- scan all stores and days
- filter those with sufficient stock
- select the minimum price among valid stores

This directly yields the optimal solution.

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

- linear scan solution with minimal overhead  
- clear separation between feasibility check and optimization  
- no need for complex data structures or algorithms  
- fully reproducible with automated tests  

---

## Notes

This problem represents a basic filtering + minimum selection pattern.

It is included for completeness and as a reference for simple constraint-based optimization problems, rather than for advanced algorithmic techniques.
