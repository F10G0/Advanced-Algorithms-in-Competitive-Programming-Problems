# Algorithm Notes

This directory contains materials explaining the core algorithm behind the PPM solution.

## Goal

Given `k` letters with positive costs `w[i]`, the task is to construct exactly `n` distinct strings such that

- no chosen string is a prefix of another
- the total cost is minimum

The cost of a string is the sum of the costs of its characters.

## Main Observation

The prefix-free constraint can be modeled using an infinite rooted `k`-ary tree.

- the root represents the empty string
- each edge corresponds to one letter
- taking an edge adds the corresponding letter cost
- each string corresponds to a root-to-node path

So the problem becomes:

> Select exactly `n` leaves in a weighted infinite `k`-ary tree so that the sum of their root-to-leaf costs is minimum.

## Leaf Expansion Reformulation

A chosen leaf with cost `x` can be expanded into `k` children with costs

- `x + w[0]`
- `x + w[1]`
- `...`
- `x + w[k-1]`

After expansion, the original leaf disappears and is replaced by its children.

This turns the problem into a dynamic leaf-set optimization problem:
we repeatedly decide whether expanding some current leaf leads to a better set of `n` leaves.

## Candidate Generation

The algorithm first sorts the letter costs increasingly.

Then it starts from the root cost `0` and repeatedly expands the current minimum leaf.
This generates cheap reachable leaf costs in increasing order.

Once at least `n` non-empty leaves have been produced, the largest extra leaves are removed, leaving exactly `n` candidates.

This phase gives a good initial leaf set, but it is not yet guaranteed to be globally optimal.

## Greedy Improvement

The second phase tries to improve the current solution by expanding the smallest current leaf.

Suppose the minimum leaf has cost `x`.

If it is expanded:

- `x` is removed
- new children `x + w[i]` are introduced
- to keep exactly `n` leaves, some current maximum leaves must be discarded

For each additional child, the algorithm compares

- the cost of the new child
- the cost of the current maximum leaf

A replacement is performed only if it decreases the total sum.

## Core Insight

The smallest current leaf is always the best candidate for expansion.

If expanding the minimum leaf is not profitable, then expanding any larger leaf cannot be profitable either, because all of its children would be at least as expensive.

This gives the stopping rule:

> Once expanding the current minimum leaf no longer improves the total cost, the current leaf set is already optimal.

## Data Structure

The implementation needs both of the following efficiently:

- extract the minimum leaf
- extract the maximum leaf

The final solution uses a min-max heap, which supports both operations in `O(log n)` time.

This is the key engineering improvement over earlier two-heap versions.

## Final Algorithm

1. Read `n`, `k`, and the letter costs.
2. Sort the costs increasingly.
3. Insert the root cost `0`.
4. Repeatedly expand the current minimum leaf until at least `n` non-empty leaves exist.
5. Remove the largest leaves until exactly `n` remain.
6. Repeatedly test whether expanding the current minimum leaf decreases the total sum.
7. Stop when no further profitable expansion exists.
8. Output the maintained total cost.

## Files

- `ppm_idea.md`: core explanation of the tree model and greedy strategy
- `ppm_annotated_en.c`: English annotated final implementation
- `algorithm.pdf`: detailed technical write-up

## Relationship to the Final Implementation

The final executable version is located in `../src/ppm.c`.

The files in this directory explain the reasoning behind the final min-max heap solution, from the tree-based reformulation to the greedy expand-and-replace process.
