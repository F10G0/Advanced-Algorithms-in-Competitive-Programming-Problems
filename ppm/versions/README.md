# Historical Versions — PPM

This directory contains earlier implementations of the same core algorithmic idea.

All versions follow the same high-level strategy:

- model the problem as selecting leaves in a weighted k-ary tree
- maintain a set of n candidate leaf costs
- repeatedly try to improve the set by expanding the smallest leaf and replacing the largest ones

The difference between versions lies in how the data structure is implemented.

---

## Versions

### ppm_two_heaps_lazy.c

Baseline implementation using two heaps:

- a min-heap for extracting the smallest leaf
- a max-heap for removing the largest leaf
- lazy deletion to synchronize the two heaps

Characteristics:

- simple and intuitive
- directly reflects the greedy logic
- accumulates invalid elements over time

---

### ppm_two_heaps_rebuild.c

Improved two-heap implementation with periodic rebuild:

- still uses min-heap + max-heap
- introduces a rebuild step to remove invalid elements
- keeps memory usage under control

Characteristics:

- more stable than lazy-only version
- still relatively straightforward
- slightly more complex due to rebuild logic

---

## Final Version

The final optimized implementation is located in:

    ../src/ppm.c

It replaces the two-heap structure with a single min-max heap.

Advantages:

- supports both pop_min and pop_max directly
- no need for lazy deletion
- no need for rebuild
- cleaner logic and better constant factors

---

## Evolution Summary

The development path of this solution is:

    two heaps + lazy deletion
            ↓
    two heaps + rebuild
            ↓
    min-max heap (final)

This progression improves both clarity and efficiency while preserving the same greedy strategy.
