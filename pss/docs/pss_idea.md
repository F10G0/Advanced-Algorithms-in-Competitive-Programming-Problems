# Core Idea — Maximum Enjoyment with Non-Repeating Scents

This problem asks for the maximum total weight of **distinct scents** in a contiguous subarray.

The difficulty lies in the fact that each scent contributes **at most once**, regardless of how many times it appears inside the interval.

---

## 1. Key Reformulation

Instead of enumerating all intervals `[L, R]`, we fix the right endpoint `R = i`.

Define:

    A_i(L) = enjoyment of interval [L, i]

Then the problem becomes:

    for i = 1..n:
        maintain A_i(1), A_i(2), ..., A_i(i)
        take max over all L

So we reduce a 2D problem `(L, R)` into a **1D sweep over R**, while maintaining all possible starting points.

---

## 2. What Makes This Hard

For a fixed interval `[L, i]`, the contribution of a scent `j` is:

    w[j] if j appears at least once in [L, i]
    0    otherwise

So we are not summing over positions, but over **distinct elements**.

This prevents direct use of prefix sums.

---

## 3. Core Insight

Focus on one scent `j`.

Let its occurrences be:

    p1 < p2 < p3 < ...

At any time, the contribution of scent `j` across all possible left endpoints `L`
can be represented as a **range on the L-axis**.

---

## 4. Contribution as Interval on L-axis

When processing occurrence `pk`, it contributes to:

    L in (p_{k-1}, p_k]  →  [p_{k-1}+1, p_k]

Why?

- For these starting points, `pk` is the **first occurrence** of `j` inside `[L, i]`
- So these intervals should gain `+w[j]`

---

## 5. Contribution Transfer (Critical Step)

When a new occurrence appears, the responsibility shifts.

Suppose current position is `i`, and:

    l2 < l1 < i

are the last two occurrences.

### Old contribution (must be removed)

    L in [l2+1, l1]

These were previously handled by `l1`, so:

    subtract w[j]

---

### New contribution (must be added)

    L in [l1+1, i]

These are newly covered by the current occurrence, so:

    add w[j]

---

## 6. Final Update Rule

For each position `i` with scent `j`:

    l1 = last[j]
    l2 = last_last[j]

    if l1 exists:
        update [l2+1, l1] -= w[j]

    update [l1+1, i] += w[j]

    last_last[j] = l1
    last[j] = i

This maintains correct contributions for scent `j` across all starting points.

---

## 7. Data Structure

We maintain an array over all possible left endpoints:

    A_i(L)

Operations needed:

- range add
- global maximum query

→ Use a **lazy segment tree**

At each step:

    answer = max(answer, max_L A_i(L))

---

## 8. Why This Works

Each scent contributes exactly once to each interval.

By always assigning responsibility to the **latest occurrence**, and removing the previous one,
we ensure:

- no double counting
- no missing contributions

All scents are independent, so contributions add linearly.

---

## 9. Complexity

- Each position performs ≤ 2 range updates
- Each update costs O(log n)

Total:

    Time:   O(n log n)
    Memory: O(n + m)

---

## 10. One-Line Summary

Scan the right endpoint, and for each scent shift its contribution interval on the left-endpoint axis using last / second-last occurrences, while maintaining all answers with a segment tree.
