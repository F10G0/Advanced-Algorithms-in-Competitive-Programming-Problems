# Algorithm Notes

This directory contains materials explaining the core algorithm behind the PHR solution.

## Goal

Given a decimal value `x` in `[0, 1]` with fixed precision, the task is to find the reduced fraction `p / q` with the smallest possible denominator `q` such that `p / q` rounds to `x`.

## Main Observation

Instead of searching directly over fractions, we first convert the input decimal into a valid rounding interval `[l, r)`.

If the decimal is represented as an integer `v` with `p` digits after the decimal point, then all valid fractions must satisfy

- `l = (2v - 1) / (2 * 10^p)`
- `r = (2v + 1) / (2 * 10^p)`

So the problem becomes:

> Find the simplest fraction inside the interval `[l, r)`.

## Stern–Brocot Reformulation

The Stern–Brocot tree contains every positive reduced fraction exactly once.

In this tree, fractions with smaller denominators appear closer to the root.  
Therefore, finding the minimum denominator is equivalent to finding the shallowest node whose value lies inside the interval.

## Path Encoding

Each boundary fraction can be represented by a path in the Stern–Brocot tree using alternating `L` and `R` segments.

Instead of storing every single step, the implementation compresses consecutive moves into segment lengths, which makes the representation shorter and easier to compare.

This compressed path can be generated efficiently using an Euclidean-style process.

## Core Insight

Let the compressed Stern–Brocot paths of `l` and `r` be given.

Then the answer is determined by the longest common prefix (LCP) of these two paths.

Intuitively, the common prefix identifies the deepest shared ancestor of both boundaries, which corresponds to the shallowest fraction that still lies inside the valid interval.

## Open Right Boundary

The interval is half-open: `[l, r)`.

This means the right endpoint must not be chosen directly.  
To handle this cleanly, the implementation extends the path of `r` so that comparisons still produce a fraction strictly smaller than `r`, while remaining as close as possible to the boundary.

## Final Algorithm

1. Parse the decimal input as integer parameters `(v, p)`.
2. Construct the rounding interval `[l, r)`.
3. Encode both interval boundaries as compressed Stern–Brocot paths.
4. Compute their longest common prefix.
5. Reconstruct the corresponding fraction and output its denominator.

## Files

- `path_prototype.c`: minimal prototype for Stern–Brocot path encoding
- `phr_annotated_en.c`: English annotated implementation
- `phr_annotated_zh.c`: Chinese annotated implementation
- `algorithm.pdf`: detailed technical write-up

## Relationship to the Final Implementation

The final executable version is located in `../src/phr.c`.

The files in this directory document the reasoning process from the basic path-encoding prototype to the complete interval-based solution.
