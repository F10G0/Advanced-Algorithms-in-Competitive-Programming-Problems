# TAS — Tangential Rectangle Groups

This project implements an efficient algorithm to identify the largest tangential group of rectangles.

The solution is based on a sweep-line approach combined with an AVL tree and union-find.

---

## Problem

Given a set of axis-aligned rectangles, determine the tangential group with maximum total area.

Two rectangles are considered connected if they touch

- along an edge, or  
- at a single corner  

The output consists of

- the total area of the largest group, and  
- the indices of all rectangles in that group.

---

## Key Idea

Instead of checking all pairs of rectangles, we:

- process rectangle boundaries using a sweep line on the x-axis  
- maintain active rectangles ordered by y using an AVL tree  
- detect contacts locally (active set + ENTER/EXIT matching)  
- merge components using union-find  

This avoids quadratic complexity and reduces the problem to local event-based processing.

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

- efficient `O(n log n)` sweep-line solution  
- explicit handling of both edge and corner contacts  
- AVL-based active set for ordered interval management  
- union-find with on-the-fly area aggregation  
- multiple implementations (bruteforce, spatial hash, optimized)

---

## Documentation

- `docs/README.md` — concise algorithm overview  
- `docs/algorithm.pdf` — detailed technical write-up  

---

## Notes

This project emphasizes clean geometric modeling and the integration of multiple classical techniques (sweep line, balanced BST, union-find) into a single efficient solution.
