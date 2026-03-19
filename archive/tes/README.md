# TES — Minimum Treats Exchange via Net Balance

This project implements an efficient algorithm to minimize the total number of treats exchanged in a system of debts.

The solution is based on reducing edge-based debts to node-level net balances and performing a greedy matching between payers and receivers.

---

## Problem

Given a set of debt relations between entities, where each relation specifies that one entity owes another a certain number of treats, determine:

- the minimum total number of treats that must be exchanged to settle all debts  
- a valid sequence of exchanges achieving this minimum  

---

## Key Idea

Instead of processing each debt individually, we:

- compute the net balance for each node  
- interpret negative balances as payers and positive balances as receivers  
- match payers to receivers greedily  

The minimal total exchange equals the sum of all positive balances.

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

- linear-time solution based on net balance reduction  
- avoids unnecessary intermediate transfers  
- simple and robust greedy matching strategy  
- fully reproducible with automated tests  

---

## Notes

This problem illustrates a fundamental modeling technique: collapsing edge-based flow into node-level net balances.  
The focus is on clarity and correctness rather than algorithmic complexity.
