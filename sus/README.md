# SUS — System for Universal Signalling

This project implements an efficient algorithm to compute the minimum cost of signal propagation to all key nodes in a graph under asymmetric propagation rules.

The solution is based on graph decomposition into shortest-path access and minimum spanning tree (MST) propagation.

---

## Problem

Given a weighted undirected graph with designated key nodes, each query starts from a node `s`.

The signal propagation follows:

- non-key nodes can forward the signal to at most one neighbor  
- key nodes can broadcast to multiple neighbors  

Compute the minimum total cost required to propagate the signal to all key nodes.

---

## Key Idea

Instead of simulating the propagation process directly, we:

- compute the minimum distance from the query node to any key node  
- compute the minimum cost to connect all key nodes  

This reduces the problem to a combination of shortest path and minimum spanning tree.

The final answer is:

answer = access_cost + mst_cost

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

- non-trivial graph modeling from propagation rules  
- combination of shortest path and MST techniques  
- multi-source Dijkstra for efficient preprocessing  
- O(1) query after preprocessing  
- clean and modular C implementation  

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This project emphasizes problem modeling and algorithmic decomposition, rather than brute-force simulation of the propagation process.
