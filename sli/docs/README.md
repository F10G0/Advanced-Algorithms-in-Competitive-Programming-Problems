# Algorithm Notes

This directory contains materials explaining the core algorithm behind the SLI solution.

## Goal

Given a row of `n` slots, a starting position `s`, and directional costs for each of the `n - 1` moves, the task is to:

- minimize the total effort required to visit all slots, and
- output one valid visiting order achieving this minimum.

For move `i`:

- moving left costs `l[i]`
- moving right costs `r[i]`

Since a single move may jump across any number of slots, the cost depends only on the chosen direction, not on the distance traveled.

## Main Observation

The movement length is irrelevant.

At every step, the only real decision is whether to expand the already visited interval to the left or to the right.

Starting from slot `s`, the visited set always remains a contiguous interval.  
So the problem is not about arbitrary motion on a line, but about choosing a sequence of `left` / `right` expansions.

## Greedy Reformulation

If feasibility is ignored, then each move can be optimized independently.

For move `i`, the locally optimal choice is simply:

- go left if `l[i] < r[i]`
- go right if `r[i] <= l[i]`

This gives the baseline cost:

- `sum(min(l[i], r[i]))`

We also define the flip cost:

- `d[i] = |l[i] - r[i]|`

which is the extra price paid if move `i` is forced to use the non-greedy direction.

## Feasibility Constraint

A purely greedy sequence may fail to visit all slots.

To cover the whole row, the strategy must eventually expand both sides of the starting position:

- some moves must contribute to reaching slots left of `s`
- some moves must contribute to reaching slots right of `s`

If the greedy sequence misses one side entirely, then it is invalid even if its local choices are cheapest.

## Core Insight

Only one repair is ever needed.

If the greedy direction sequence is infeasible, the problem is not that many moves are wrong.  
It only lacks one necessary directional expansion.

Therefore, the optimal correction is:

> flip exactly one move whose direction restores feasibility and whose additional cost is minimum.

So the final answer is:

- greedy baseline cost, plus
- the cheapest valid single flip if such a repair is necessary

## Path Construction

After the final direction sequence is fixed, we still need to output an actual visiting order.

The implementation does not simulate arbitrary jumps directly.  
Instead, it reconstructs the order using interval boundaries.

The direction sequence is compressed into maximal consecutive segments of equal direction.  
Then the algorithm maintains the remaining left and right boundaries of unvisited slots and outputs the corresponding slots segment by segment.

The starting slot `s` is skipped exactly once when the constructed order crosses it.

## Final Algorithm

1. Read all move costs.
2. Choose the cheaper direction greedily for each move.
3. Accumulate the baseline cost `sum(min(l[i], r[i]))`.
4. Check whether the greedy direction pattern can visit both sides of the starting position.
5. If necessary, flip the cheapest move that restores feasibility.
6. Compress the final direction sequence into contiguous segments.
7. Reconstruct and output one valid visiting order.

## Files

- `sli_annotated_en.c`: English annotated implementation of the final algorithm
- `sli_alternative.c`: earlier implementation with a different feasibility-handling strategy
- `algorithm.pdf`: detailed technical write-up

## Relationship to the Final Implementation

The final executable version is located in `../src/sli.c`.

The annotated version `sli_annotated_en.c` and the final implementation in `src/` use the same core logic.  
The version in `docs/` is kept for explanation, while the version in `src/` is intended for direct compilation and submission.
