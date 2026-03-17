# Algorithm Notes

This directory contains materials explaining the core algorithm behind the TPE solution.

## Goal

Given a string `s` and multiple queries, each query specifies a substring `s[l..r]`.

The task is to compute the **shortest period length** of each substring.

---

## Main Observation

Let the substring be:

```
w = s[l..r]
len = r - l + 1
```

If `p` is a valid period of `w`, then:

```
p | len
```

So instead of checking all possible values, we only need to consider:

```
p ∈ divisors(len)
```

This reduces the problem from a general string problem to a divisor enumeration problem.

---

## Periodicity Reformulation

A divisor `p` is a valid period if and only if:

```
s[l..r-p] == s[l+p..r]
```

This condition ensures that every character matches the one `p` positions ahead.

So the problem becomes:

> For each divisor `p`, check whether two shifted substrings are equal.

---

## Hash-Based Optimization

Direct substring comparison is too slow.

We use rolling hash to support constant-time substring equality checks:

```
hash(l, r) = H[r] - H[l-1] * BASE^(r-l+1)
```

Thus, each periodicity test becomes:

```
hash(l, r-p) == hash(l+p, r)
```

This allows each candidate divisor to be checked in `O(1)` time.

---

## Divisor Preprocessing

Each query requires the divisors of `len`.

Instead of computing them repeatedly, we preprocess all divisor lists for lengths `1..n`:

```
for d = 1..n:
    for multiple = d, 2d, 3d, ...:
        d is a divisor of multiple
```

This runs in:

```
O(n log n)
```

and produces divisors in increasing order.

---

## Core Insight

The problem becomes efficient only after the following transformation:

```
substring periodicity
-> divisors of length
-> shifted substring equality
-> hash comparison
```

This allows us to avoid per-query linear work.

---

## Final Algorithm

For each query `(l, r)`:

1. Compute `len = r - l + 1`
2. Enumerate all divisors `p` of `len` in increasing order
3. For each `p`, check:

```
hash(l, r-p) == hash(l+p, r)
```

4. Output the first valid `p`

Since `len` itself is always a valid period, the process always terminates.

---

## Files

- `tpe_idea.md`: core idea and problem reformulation
- `tpe_annotated_en.c`: English annotated implementation
- `tpe_annotated_zh.c`: Chinese annotated implementation
- `tpe_kmp.c`: baseline solution (per-query KMP)
- `tpe_divisor.c`: intermediate solution (per-query divisor computation)
- `algorithm.pdf`: detailed technical write-up

---

## Relationship to the Final Implementation

The final executable version is located in `../src/tpe.c`.

The files in this directory document the progression from naive approaches to the optimized solution using hashing and divisor preprocessing.
