# PPM — Algorithm Idea

## Problem Restatement

We are given:

- k letters, each with a cost w[i]
- an integer n

We must construct exactly n distinct strings such that:

- no string is a prefix of another
- the total cost is minimized

The cost of a string is the sum of its character costs.

---

## Key Modeling: Prefix-Free Set → Tree

The prefix constraint immediately suggests a tree model.

### Interpretation

- Root = empty string
- Each edge corresponds to a letter with cost w[i]
- A string = path from root to a node
- Cost = sum of edge weights

### Important constraint

No string is a prefix of another  
→ all chosen strings must be leaves

---

## Reformulation

We convert the problem into:

Select exactly n leaves in an infinite k-ary tree such that the sum of root-to-leaf costs is minimized.

This is the core abstraction.

---

## Core Operation: Expand a Leaf

Suppose we have a leaf with cost x.

If we expand it:

- the leaf x disappears
- it is replaced by k new leaves:

    x + w[0], x + w[1], ..., x + w[k-1]

This is the only transformation we need.

---

## High-Level Strategy

The algorithm has two phases:

    Phase 1: generate cheap candidate leaves
    Phase 2: greedily improve structure

---

## Phase 1 — Generate Candidate Leaves

We simulate expansion starting from the root:

1. Start with leaf 0
2. Repeatedly:
   - take the smallest leaf x
   - expand it into x + w[i]

This produces leaves in increasing cost order.

### Goal

We want at least n leaves.

Then:

- if more than n, remove the largest ones
- keep the smallest n leaves

At this point:

we have the n cheapest reachable leaf costs,  
but not necessarily a valid optimal structure.

---

## Phase 2 — Greedy Improvement

Now we try to improve the current set.

### Key idea

Try expanding the smallest leaf.

---

### Suppose smallest leaf is x

If we expand it:

- we lose x
- we gain:

    x + w[0], x + w[1], ..., x + w[k-1]

But we must keep exactly n leaves → we must remove some largest leaves.

---

### Cost change analysis

1. First child is mandatory:

    d = w[0]

2. For each additional child:

    delta = (x + w[i]) - current_max

- if delta < 0 → beneficial → replace
- else → stop

We accumulate:

    d += delta

---

### Decision rule

- if d < 0 → expansion is profitable → commit
- else → stop completely

---

## Why Only Expand the Minimum Leaf?

This is the key greedy argument.

- expanding a leaf of cost x produces children >= x + w[0]
- if we expand a larger leaf, all its children are worse

Therefore:

if expanding the smallest leaf is not beneficial,  
then expanding any other leaf is also not beneficial

This guarantees correctness.

---

## Data Structure

We need:

- extract minimum
- extract maximum
- insert values

### Solution: Min-Max Heap

Supports:

- pop_min() → expand candidate
- pop_max() → remove worst leaf

All in O(log n).

---

## Algorithm Summary

1. Sort w[i]
2. Initialize heap with 0
3. Expand smallest leaf until >= n leaves
4. Remove largest until exactly n
5. Repeat:
   - try expanding smallest leaf
   - if beneficial → apply
   - else → stop
6. Output sum

---

## Complexity

Each operation is O(log n)

Total complexity:

    O(n log n)

---

## Key Insights

### 1. Prefix-free → leaf selection

Transforms strings into a structural optimization problem.

---

### 2. Not just "take n smallest paths"

Because:

- expanding a node removes it
- adds multiple children
- structure matters

---

### 3. Greedy = expand + replace

- expand cheapest leaf
- replace worst leaves if beneficial

---

### 4. Two-phase structure

    generate candidates → refine to optimal

This pattern appears in many advanced greedy problems.

---

## Implementation Evolution (Important)

This problem naturally leads to multiple implementations:

    two heaps + lazy deletion
            ↓
    two heaps + rebuild
            ↓
    min-max heap (final)

Final version:

- simpler
- no invalid elements
- better constants

---

## Final Insight

We are not directly constructing strings.  
We are dynamically maintaining the optimal set of leaves in a weighted infinite tree.

This abstraction is the key to solving the problem efficiently.
