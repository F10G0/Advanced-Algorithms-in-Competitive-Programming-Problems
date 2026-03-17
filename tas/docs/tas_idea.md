# TAS – Idea

## 1. From Geometry to Connectivity

The problem is stated in geometric terms: we are given axis-aligned rectangles, and two rectangles are connected if they are *tangential*, i.e., they touch either

- along a boundary segment, or  
- at a single corner point.

The key transformation is:

> Treat each rectangle as a node, and connect two nodes if the corresponding rectangles touch.

Then the task becomes:

- find connected components,
- compute the total area of each component,
- return the component with maximum area.

So the core difficulty is not union-find, but:

> How to efficiently detect all touching pairs.

---

## 2. Why Brute Force Fails

A direct approach checks every pair of rectangles:

- time complexity: `O(n^2)`

This is too slow for large `n`.

We need to avoid global pairwise comparison and exploit geometric structure.

---

## 3. Key Observation: Contacts Are Local in x

Each rectangle can be written as:

- x-interval: `[x1, x2]`
- y-interval: `[y1, y2]`

Two rectangles can only touch if:

- their x-intervals overlap or meet, and  
- their y-intervals overlap or meet.

This suggests using a sweep line along the x-axis.

---

## 4. Sweep Line Decomposition

We sweep from left to right.

For each rectangle:

- ENTER event at `x1`
- EXIT event at `x2`

At each x-position, rectangles are:

- active (currently overlapping x)
- entering
- exiting

We process each x as:

1. remove EXIT rectangles  
2. match ENTER with EXIT  
3. insert ENTER rectangles  

---

## 5. Two Types of Contacts

### 5.1 Horizontal Contact (inside active set)

If two rectangles overlap in x, they may touch vertically:

- `y2(A) == y1(B)` or  
- `y1(A) == y2(B)`

These are detected when inserting into the active set.

---

### 5.2 Vertical Contact (ENTER vs EXIT)

If:

- `x2(A) == x1(B)`

then one rectangle exits and another enters at the same x.

To detect contacts:

- sort ENTER and EXIT by y  
- use two-pointer merge  
- detect overlapping or touching y-intervals  

This also covers corner contacts.

---

## 6. Active Set (AVL Tree)

We maintain active rectangles ordered by `y1`.

Operations:

- insert rectangle
- delete rectangle

Key idea:

> Since rectangles do not overlap in area, only boundary alignment needs to be checked.

During insertion:

- if new `y2 == existing y1` → contact  
- if new `y1 == existing y2` → contact  

This is checked along the insertion path.

---

## 7. Connectivity via Union-Find

Each rectangle starts as its own component.

Whenever a contact is found:

- merge the two components
- maintain total area at the root

Thus, connectivity and area are updated online.

---

## 8. Corner Contacts

Corner contacts happen when:

- both x and y boundaries align at a single point

They are automatically detected:

- in ENTER–EXIT matching  
- in active set boundary checks  

No special handling is required.

---

## 9. Overall Insight

The key idea is:

> Replace global pairwise checking with local event-based processing.

- sweep line localizes interactions in x  
- AVL tree organizes structure in y  
- union-find maintains connectivity  

This reduces the problem to:

- time complexity: `O(n log n)`

---

## 10. Takeaway

This problem follows a standard pattern:

- geometry → events → data structure → connectivity

Key techniques:

- sweep line
- balanced BST (AVL)
- union-find

The combination yields an efficient and clean solution.
