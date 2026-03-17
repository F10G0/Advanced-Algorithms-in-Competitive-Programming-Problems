# Algorithm Notes

This directory contains materials explaining the core algorithm behind the RAI solution.

## Goal

Given $n$ glades, each with initial value $b_i$ and daily growth $a_i$, determine an ordering such that for every prefix length $k$, the total collected value over the first $k$ days is maximized.

## Main Observation

Each glade can be modeled as a linear function

$$
f(t) = a \cdot t + b.
$$

Thus, the problem becomes:

> Construct an ordering of functions such that every prefix sum is maximized.

## Ordering Reformulation

For two functions $f$ and $g$, their relative order depends on the day $t$ at which they are evaluated.

At position $t$, we prefer $f$ over $g$ if

$$
f(t) > g(t)
\quad \Longleftrightarrow \quad
a_f t + b_f > a_g t + b_g.
$$

This leads to an ordering problem where each position imposes a comparison between functions.

## Naive Construction

We build the sequence incrementally:

1. Sort all functions by increasing slope $a$ (and decreasing $b$ if equal).
2. Insert each function into the earliest position where it becomes preferable.

When a function is inserted at position $i$, all subsequent functions are shifted to the right by one day.

## Shift Effect

If a function

$$
g(t) = a_g t + b_g
$$

is shifted from day $t$ to $t+1$, then

$$
g(t+1) = a_g t + (b_g + a_g).
$$

Thus, shifting by one day increases its intercept by $a_g$.

During insertion, this results in an effective update of the form

$$
b_g \leftarrow b_g + a_f - a_g,
$$

which corresponds to a suffix update.

## Data Structure

To support efficient insertion and updates, we maintain the sequence in a splay tree.

Each node stores:
- slope $a$
- intercept $b$
- subtree size

The in-order traversal of the tree corresponds to the final day ordering.

## Lazy Propagation

After inserting a function $f$, all nodes in the right subtree are shifted by one day.

This is implemented using lazy propagation with accumulated updates applied as

$$
b \leftarrow b + \text{push\_val} - a \cdot \text{delay\_days}.
$$

## Core Insight

The naive insertion approach requires explicit updates over all affected elements, leading to $O(n^2)$ complexity.

By representing the sequence as a splay tree and applying suffix updates lazily, the same logic can be implemented in $O(n \log n)$ time.

## Files

- `rai_idea.md`: intuition and modeling motivation  
- `rai_naive_insertion.c`: naive insertion-based prototype  
- `rai_annotated_en.c`: annotated optimized implementation  
- `rai_annotated_zh.c`: Chinese annotated implementation  
- `algorithm.pdf`: detailed technical write-up  

## Relationship to the Final Implementation

The final executable version is located in `../src/rai.c`.

The files in this directory document the transition from the high-level modeling idea to the optimized splay-based solution.
