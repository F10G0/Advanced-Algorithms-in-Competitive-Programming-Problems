# Algorithm Notes

This directory contains materials explaining the core algorithm behind the PSS solution.

## Goal

Given a sequence of scents `f[1..n]`, where each scent `j` has a weight `w[j]`,  
the task is to find a contiguous interval `[L, R]` such that the sum of weights of **distinct scents** appearing in the interval is maximized.

Each scent contributes its value at most once within the interval, regardless of how many times it appears.

---

## Main Observation

Instead of enumerating all intervals `[L, R]`, we fix the right endpoint `R = i`.

For each `i`, define:

    A_i(L) = enjoyment of interval [L, i]

Then the problem becomes:

> For each right endpoint `i`, compute `max_L A_i(L)`, and take the global maximum.

This reduces the problem to maintaining all possible starting points while scanning `i` from left to right.

---

## Contribution Modeling

For each scent `j`, its contribution depends only on whether it appears in `[L, i]`.

Let the occurrences of scent `j` be:

    p1 < p2 < p3 < ...

At any time, its contribution across all left endpoints can be represented as a **range on the L-axis**.

---

## Contribution Transfer

When processing a new occurrence at position `i`, let:

    l1 = last occurrence of j
    l2 = second last occurrence of j

Then:

- The previous contribution interval:

      [l2+1, l1]

  must be removed (−w[j])

- The new contribution interval:

      [l1+1, i]

  must be added (+w[j])

This shifts the responsibility of contributing `w[j]` from the previous occurrence to the current one.

---

## Data Structure

We maintain all values `A_i(L)` using a segment tree.

Supported operations:

- range addition
- global maximum query

At each step:

    answer = max(answer, max_L A_i(L))

---

## Core Insight

Each scent contributes exactly once to any interval.

By assigning the contribution responsibility to the **most recent occurrence**,  
and transferring it when a new occurrence appears, we ensure:

- no double counting
- correct coverage of all intervals

This allows us to maintain all interval values incrementally.

---

## Final Algorithm

1. Initialize arrays `last[]` and `last_last[]` for all scents.
2. Iterate `i = 1..n`:
   - Let `j = f[i]`
   - Retrieve `l1 = last[j]`, `l2 = last_last[j]`
   - If `l1 > 0`, update:

         [l2+1, l1] -= w[j]

   - Update:

         [l1+1, i] += w[j]

   - Update occurrence records:

         last_last[j] = l1
         last[j] = i

   - Query segment tree root for current maximum
3. Output the global maximum

---

## Complexity

    Time:   O(n log n)
    Memory: O(n + m)

---

## Files

- `pss_idea.md`: core modeling and insight
- `pss_annotated_en.c`: annotated implementation
- `algorithm.pdf`: detailed write-up (optional)

---

## Relationship to the Final Implementation

The final executable solution is located in:

    ../src/pss.c

The files in this directory document the transition from the abstract contribution model
to the efficient segment tree implementation.
