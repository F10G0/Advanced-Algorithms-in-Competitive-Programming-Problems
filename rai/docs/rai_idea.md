# RAI - Rainy Days

## 1. Problem Restatement

We are given $n$ glades.  
Each glade $i$ has:

- Initial mushrooms: $b_i$
- Daily growth: $a_i$

If we harvest glade $i$ on day $t$ (0-indexed), the collected mushrooms are:

$$
f_i(t) = b_i + a_i \cdot t
$$

### Goal

For each $k = 1, 2, \dots, n$, compute the **maximum total mushrooms** obtainable in exactly $k$ days.

---

## 2. Key Transformation

Each glade can be modeled as a linear function:

$$
f_i(t) = a_i \cdot t + b_i
$$

The problem becomes:

> Find an ordering of functions such that for every prefix $k$,  
> the sum
>
> $$
> \sum_{j=0}^{k-1} f_{\pi(j)}(j)
> $$
>
> is maximized.

This is a **prefix-optimal sequence construction problem**.

---

## 3. Why Greedy Fails

A naive greedy approach would choose the glade with the maximum value at each day.

However, this fails because:

- Choosing a glade early reduces its future potential
- Some glades grow faster (large $a_i$) and should be delayed

Thus, we need a **global ordering**, not a step-by-step greedy decision.

---

## 4. Naive Solution (O(n²))

### Idea

Construct the sequence incrementally:

1. Sort all functions by:
   - Increasing $a$
   - If equal, decreasing $b$

2. Maintain a sequence

3. Insert each function into the earliest position where it becomes better

---

### Insertion Rule

At position $t$, insert function $f$ before $g$ if:

$$
f(t) > g(t)
\quad \Longleftrightarrow \quad
a_f \cdot t + b_f > a_g \cdot t + b_g
$$

---

### Adjustment

When inserting at position $i$:

- All functions after position $i$ shift right by one day
- Their value changes accordingly

This requires updating all affected elements → $O(n)$ per insertion

---

### Complexity

$$
O(n^2)
$$

Implementation:  
`rai_naive_insertion.c`

---

## 5. Key Optimization Insight

When shifting a function $g(t) = a_g t + b_g$ to day $t+1$:

$$
g(t+1) = a_g (t+1) + b_g = a_g t + (b_g + a_g)
$$

Thus, shifting by one day is equivalent to:

$$
b_g \leftarrow b_g + a_g
$$

---

### With New Function $f$

Due to insertion mechanics, the actual transformation becomes:

$$
b_g \leftarrow b_g + a_f - a_g
$$

This is a **range update over a suffix**.

---

## 6. Data Structure Design

We need to support:

- Ordered sequence (by position)
- Insert at arbitrary position
- Apply updates to a suffix

### Solution

Use a **Splay Tree with Lazy Propagation**

---

### Node Structure

Each node represents a function:

- $a$: slope
- $b$: intercept
- `size`: subtree size

Lazy tags:

- `push_val`: accumulated constant shift
- `delay_days`: number of day shifts

---

### Tree Property

- In-order traversal = day order
- Position in tree = day index

---

## 7. Insertion Logic

While inserting function $f$:

1. Traverse the tree
2. Maintain current day $t$
3. Compare:

$$
f(t) \stackrel{?}{>} g(t)
$$

4. Decide:
   - Go left → insert earlier
   - Go right → insert later

---

## 8. Lazy Propagation

After inserting $f$:

All nodes in the **right subtree** shift by one day.

Apply:

$$
\text{push\_val} += a_f
$$

$$
\text{delay\_days} += 1
$$

When propagating:

$$
b \leftarrow b + \text{push\_val} - a \cdot \text{delay\_days}
$$

---

## 9. Complexity Analysis

- Sorting: $O(n \log n)$
- Each insertion: amortized $O(\log n)$
- Lazy updates: $O(1)$

### Total:

$$
O(n \log n)
$$

---

## 10. Final Computation

Perform an **in-order traversal**:

- Day index $t = 0,1,2,\dots$
- Evaluate each function:

$$
f(t) = a \cdot t + b
$$

- Compute prefix sums

---

## 11. Key Takeaways

- Transform problem → linear functions
- Construct **prefix-optimal ordering**
- Naive insertion → $O(n^2)$
- Optimize using:
  - Splay tree (order maintenance)
  - Lazy propagation (range update)

---

## 12. Related Concepts

- Scheduling with time-dependent rewards
- Slope-based ordering
- Self-adjusting trees (Splay Tree)
- Lazy propagation

---

## 13. File References

- Optimized solution: `src/rai.c`
- Naive version: `docs/rai_naive_insertion.c`
- Annotated versions:
  - `docs/rai_annotated_en.c`
  - `docs/rai_annotated_zh.c`

---

## 14. Additional Resources

For a more formal derivation and extended discussion:

- `algorithm.pdf`
