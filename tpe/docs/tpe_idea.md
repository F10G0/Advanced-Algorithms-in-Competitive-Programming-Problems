# TPE – Idea

## 1. Problem Reformulation

We are given a string `s` and many queries.  
Each query asks for the **shortest period length** of a substring `s[l..r]`.

Let

```
w = s[l..r]
len = r - l + 1
```

A positive integer `p` is a valid period length of `w` if `w` is composed of repetitions of its prefix of length `p`.

---

## 2. Key Observation

If `p` is a period of `w`, then `p` must divide `len`.

So instead of checking all possible values, we only need to check the divisors of `len`.

```
p is a period  =>  p | len
```

This is the crucial reduction:

```
string problem -> divisor enumeration problem
```

---

## 3. Equivalent Periodicity Test

For a divisor `p` of `len`, `p` is a valid period if and only if

```
s[l..r-p] == s[l+p..r]
```

### Why this works

If `p` is a period, then for every valid position,

```
s[i] = s[i+p]
```

for all `i = l, l+1, ..., r-p`.

So the substring `s[l..r-p]` must be equal to `s[l+p..r]`.

Conversely, if these two substrings are equal, then every character matches the one `p` positions later, so the whole substring is formed by repeating its first `p` characters.

---

## 4. Fast Substring Comparison with Rolling Hash

Direct substring comparison is too slow.

We preprocess prefix hashes and powers of the hash base, so that any substring hash can be computed in `O(1)`:

```
hash(l, r) = H[r] - H[l-1] * BASE^(r-l+1)
```

Then the periodicity test becomes:

```
hash(l, r-p) == hash(l+p, r)
```

So each candidate divisor can be checked in constant time.

---

## 5. Divisor Preprocessing

Each query only needs the divisors of its substring length `len`.

Instead of recomputing divisors for every query, we preprocess the divisor list for every length from `1` to `n`.

A standard sieve-like method is:

```
for d = 1..n:
    for multiple = d, 2d, 3d, ...:
        d is a divisor of multiple
```

This runs in:

```
O(n log n)
```

and naturally produces divisors in increasing order.

That is important, because the **first valid divisor** is exactly the shortest period length.

---

## 6. Final Algorithm

### Preprocessing

1. Read the string.
2. Build prefix hashes and hash powers.
3. Precompute all divisor lists for lengths `1..n`.

### For each query `(l, r)`

```
len = r - l + 1

for p in divisors(len) in increasing order:
    if p == len:
        answer = p
        stop
    if hash(l, r-p) == hash(l+p, r):
        answer = p
        stop
```

Since the full length `len` is always a valid period, the process always terminates.

---

## 7. Complexity

### Preprocessing

- Prefix hash: `O(n)`
- Hash powers: `O(n)`
- Divisor preprocessing: `O(n log n)`

Total:

```
O(n log n)
```

### Per Query

If the substring length is `len`, we only iterate over the divisors of `len`.

So each query costs:

```
O(d(len))
```

where `d(len)` is the number of divisors of `len`.

In practice this is very small.

---

## 8. Why Simpler Approaches Are Too Slow

### Per-query KMP

A straightforward solution is to extract the substring and run a prefix-function or KMP-based period check.

That costs `O(len)` per query, which is too slow when the number of queries is very large.

### Character-by-character comparison

Testing candidate periods directly by comparing characters also leads to `O(len)` or worse per query.

Neither approach is fast enough for the input limits.

---

## 9. Core Insight

The key is not the hash itself.

The real idea is the following transformation:

```
substring periodicity
-> divisors of the length
-> shifted substring equality
-> O(1) hash comparison
```

Only after this reformulation does the problem become efficient.

---

## 10. Summary

- The shortest period must divide the substring length.
- A divisor `p` is valid iff `s[l..r-p] == s[l+p..r]`.
- Rolling hash makes each such test `O(1)`.
- Precomputing all divisor lists avoids repeated work across queries.

This yields an efficient solution with `O(n log n)` preprocessing and very fast query handling.
