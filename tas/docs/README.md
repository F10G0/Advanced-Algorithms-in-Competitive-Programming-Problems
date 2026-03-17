# Algorithm Notes

This directory contains materials explaining the core algorithm behind the TAS solution.

## Goal

Given a set of axis-aligned rectangles, the task is to identify the tangential group with maximum total area.

Two rectangles belong to the same group if they are connected through tangential contact, where touching

- along an edge, or
- at a single corner

both count as valid adjacency.

The output consists of

- the total area of the largest tangential group, and
- the indices of all rectangles in that group.

## Main Observation

The problem can be reformulated as a connectivity problem.

- Each rectangle is treated as a node.
- Two nodes are connected if the corresponding rectangles touch.
- The required answer is the connected component with maximum total area.

So the main challenge is not component maintenance itself, but efficiently detecting all touching pairs.

## Sweep-Line Reformulation

A direct `O(n^2)` pairwise check is too expensive for large inputs.

Instead, the solution processes rectangles with a sweep line along the `x`-axis.

For each rectangle, two events are created:

- `ENTER` at the left boundary
- `EXIT` at the right boundary

At each sweep position, rectangles are handled in three groups:

1. rectangles leaving the active set,
2. rectangles entering at the current `x`,
3. rectangles already active across the current `x`.

This reduces global geometric interaction to local event processing.

## Active Set Structure

Rectangles currently intersecting the sweep line are stored in an AVL tree ordered by their lower `y` coordinate.

This allows efficient

- insertion,
- deletion, and
- boundary checks against nearby active rectangles.

Because rectangles do not overlap in area, tangential contacts inside the active set can be detected through boundary alignment during insertion.

## ENTER–EXIT Matching

Not all contacts occur inside the active set.

If one rectangle ends exactly where another begins, then the two rectangles meet at the same `x` coordinate.

To detect these cases, the implementation compares

- rectangles with `EXIT` events at the current `x`, and
- rectangles with `ENTER` events at the same `x`.

After sorting both groups by `y`, a two-pointer merge detects all vertical edge contacts and corner contacts at that sweep position.

## Connectivity Maintenance

Whenever two touching rectangles are found, they are merged using union-find.

In addition to connectivity, the implementation also stores the total area of each connected component.

This makes it possible to determine the maximum tangential group directly after all events have been processed.

## Final Algorithm

1. Read all rectangles and convert them to boundary form.
2. Create `ENTER` and `EXIT` events.
3. Sort all events by `x`.
4. Sweep from left to right:
   - remove `EXIT` rectangles,
   - match `ENTER` and `EXIT` rectangles at the same `x`,
   - insert `ENTER` rectangles into the AVL tree.
5. Merge touching rectangles with union-find.
6. Find the connected component with maximum total area and output its indices.

## Files

- `tas_idea.md`: high-level modeling and algorithm design notes
- `tas_annotated_en.c`: English annotated implementation
- `tas_annotated_zh.c`: Chinese annotated implementation
- `tas_bruteforce.c`: baseline `O(n^2)` solution
- `tas_spatial_hash.c`: alternative spatial-hashing-based solution
- `algorithm.pdf`: detailed technical write-up

## Relationship to the Final Implementation

The final executable version is located in `../src/tas.c`.

The files in this directory document the reasoning process behind the final sweep-line solution, including both annotated versions and slower alternative implementations for comparison.
