# Advanced Algorithms in Competitive Programming Problems

This repository is a curated collection of solutions developed for the course **Advanced Algorithms in Competitive Programming Problems**.

Each problem is organized as a self-contained project, including implementation, test cases, and (when relevant) detailed algorithmic documentation.

The repository emphasizes not only correct solutions, but also **modeling, reduction techniques, and reproducible engineering structure**.

---

## Highlights

This repository demonstrates:

- advanced graph algorithms (SCC, MST, matching, connectivity)
- geometric algorithms (sweep line, spatial reasoning)
- advanced data structures (segment tree, splay tree, AVL)
- algorithmic design techniques (greedy, binary search, parametric search)
- mathematical methods (number theory, Stern–Brocot, Diophantine equations)

Several problems involve **multi-stage reductions**, transforming complex problem statements into classical algorithmic formulations.

---

## Repository Goals

This repository is intended to serve as:

- a structured archive of course problem solutions  
- a technical portfolio of algorithmic modeling and implementation  
- a reference for clean project organization around competitive programming problems  

The focus is on:

- **truthful documentation**
- **clear engineering structure**
- **reproducibility of results**

---

## Repository Structure

Most problem directories follow a consistent layout:

- `problem/` — original problem statement  
- `src/` — final implementation  
- `tests/` — sample inputs, outputs, and automated test scripts  
- `docs/` — algorithm explanation and annotated material  

Some problems additionally include:

- `solutions/` — alternative implementations or complexity variants  
- `versions/` — historical or intermediate solution versions  
- `templates/` — reusable components  

Additional problems with lighter documentation are stored in:

- `archive/`

---

## Suggested Entry Points

If you are reviewing this repository, the following problems are good starting points:

- `tas` — geometry + data structures (sweep line + AVL + union-find)  
- `xap` — problem reduction to bipartite matching / minimum vertex cover  
- `bur` — multi-stage modeling with multiple implementations  
- `phr` — mathematical reconstruction using Stern–Brocot trees  

---

## Problem Index

The main collection contains **17 documented problems**.

| Problem | Theme | Core Techniques | Level |
|---|---|---|---|
| `apd` | compatible set under conflicts | parameterized reduction, branching, vertex cover duality | Advanced |
| `bur` | sequential voting game | backward analysis, k-smallest maintenance, order statistics | Advanced |
| `cat` | grid travel optimization | geometric reformulation, sparse graph, Dijkstra | Core |
| `fal` | falling time under deletions | reverse processing, union-find, offline connectivity | Core |
| `grr` | alternating reachability | SCC compression, DAG reasoning | Core |
| `luc` | minimizing max wins | graph modeling, binary search, augmenting paths | Core |
| `paw` | equal-length cycle validation | Tarjan BCC, structural graph classification | Core |
| `phr` | rational reconstruction | interval reconstruction, Stern–Brocot, path LCP | Advanced |
| `ppm` | prefix-free string set | greedy tree expansion, heap optimization | Core |
| `pss` | interval distinct value maximization | sweep line, contribution transfer, lazy segment tree | Advanced |
| `rai` | time-dependent reward ordering | linear functions, splay tree, lazy propagation | Advanced |
| `sli` | minimum-effort ordering | greedy direction choice, constructive optimization | Core |
| `sus` | signalling cost minimization | Dijkstra, MST, multi-source shortest paths | Core |
| `tas` | geometric connectivity | sweep line, AVL tree, union-find | Advanced |
| `tpe` | substring periodicity | divisor enumeration, rolling hash | Core |
| `tuv` | tree detection counting | tree decomposition, interval counting, binary search | Advanced |
| `xap` | radar removal | geometric reduction, bipartite matching, vertex cover | Advanced |

---

## Archive

The `archive/` directory contains additional course problems preserved for completeness.

These problems:

- follow the same core structure (`problem/`, `src/`, `tests/`)  
- are fully runnable and correct  
- typically have lighter documentation or simpler structure  

See:

- `archive/README.md`

---

## How to Use This Repository

### Browse a problem

Open any problem directory (e.g., `tas/`, `phr/`, `bur/`).

Start with:

- `README.md` — overview  
- `docs/` — explanation and annotated material  
- `src/` — final implementation  
- `tests/` — verification  

---

### Build

Each problem provides build instructions in:

- `src/README.md`

Typical contents include:

- `Makefile`
- `build.bat`

---

### Test

Each problem provides test instructions in:

- `tests/README.md`

Typical contents include:

- `run_tests.sh`
- `run_tests.bat`
- sample input/output files  

---

## Documentation Style

This repository follows an **algorithm-explanation-oriented** documentation style.

Each problem is documented to highlight:

- the modeling step  
- the key reduction or structural insight  
- the final algorithm  
- implementation reasoning  

The goal is to preserve **how the solution is derived**, not only the final code.

---

## Notes

- Directory names use the original three-letter problem abbreviations from the course  
- Not all problems have the same level of documentation  
- Structure consistency is prioritized across all problems  

---

## Status

This repository represents a complete and structured collection of course solutions, including:

- 17 main documented problems  
- additional archived problems  
- reproducible implementations and test workflows  
