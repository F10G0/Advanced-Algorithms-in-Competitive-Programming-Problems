# Algorithm Notes

This directory contains materials explaining the core algorithm behind the SUS solution.

## Goal

Given a weighted undirected graph with designated key nodes, the task is to compute the minimum cost required to propagate a signal from a query node to all key nodes under asymmetric propagation rules.

## Main Observation

The propagation process can be decomposed into two independent components:

- a path from the query node to the key-node system  
- a broadcast process among key nodes  

Since non-key nodes cannot branch, the first part must be a shortest path.  
Once a key node is reached, branching is allowed, so the second part forms a tree structure.

## Graph Reformulation

Let `K` be the set of key nodes.

- The cost of entering the key-node system is the minimum shortest-path distance from the query node `s` to any key node in `K`.
- The cost of connecting all key nodes is obtained by constructing a complete graph on `K`, where each edge weight is the shortest-path distance between the corresponding key nodes in the original graph.

Thus, the propagation cost reduces to a minimum spanning tree on this induced graph.

## Core Insight

The total cost can be written as:

`answer = access_cost + mst_cost`

where:

- `access_cost` is the minimum distance from the query node to any key node  
- `mst_cost` is the MST cost on the key nodes  

This separation allows full preprocessing and constant-time queries.

## Multi-Source Optimization

To compute `access_cost` efficiently for all nodes, we run a multi-source Dijkstra:

- initialize all key nodes with distance `0`  
- propagate distances simultaneously  

This yields, for every node `v`:

`d[v] = min dist(v, x), x in K`

## Final Algorithm

1. Run Dijkstra from each key node to compute pairwise distances.  
2. Build the complete graph on key nodes.  
3. Compute the MST cost using Prim's algorithm.  
4. Run multi-source Dijkstra from all key nodes.  
5. For each query node `s`, output `d[s] + mst_cost`.

## Files

- `sus_idea.md`: concise explanation of the modeling and solution  
- `sus_annotated_en.c`: English annotated implementation  
- `sus_annotated_zh.c`: Chinese annotated implementation  
- `algorithm.pdf`: detailed technical write-up  

## Relationship to the Final Implementation

The final executable version is located in `../src/sus.c`.

The files in this directory document the reasoning process from graph modeling to the complete optimized solution.
