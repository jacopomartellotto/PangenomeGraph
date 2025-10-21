# 🧬 Pangenome Graph Construction and Analysis Project

![License](https://img.shields.io/badge/license-MIT-green)
![Language](https://img.shields.io/badge/language-C++-blue)
![Status](https://img.shields.io/badge/status-active-success)

This project focuses on the **construction and analysis of a pangenome graph**, a powerful data structure for representing and exploring genetic variations across multiple genomes.  
Using the **GFA (Graphical Fragment Assembly)** format, the project provides tools to read, build, and analyze labeled directed graphs, enabling efficient path traversal and pattern search.

---

## 🎯 Project Objectives

### I. Graph Reading and Construction

- Read a GFA file and build a **labeled, directed graph** `G` using adjacency lists in C++.
- Assign labels to nodes and edges of `G` based on the information provided in the GFA file.
- Consider only:
  - `S` (Segment) entries for graph nodes.
  - `L` (Link) entries for graph edges.

---

### II. Graph Analysis

- Determine whether `G` contains cycles.  
  - If the graph is cyclic, perform a **DFS traversal** and remove **back edges** to transform it into a **DAG (Directed Acyclic Graph)**.
- Identify a **source node `s`** (in-degree = 0) and a **destination node `t`** (out-degree = 0).  
  - If multiple sources or destinations exist, select one for analysis.

---

### III. Pattern Search on DAG Paths

- Perform a **DFS traversal** on the DAG `G` starting from `s` to enumerate all paths from `s` to `t`.  
  - The number of paths can be exponential, so this analysis is intended for small graphs.
  - Since the graph is acyclic, the boolean `visited` array can be omitted.
  - Use a character array `S` as auxiliary storage:  
    - Append the string of the current node `u` upon visiting it.  
    - Remove it once the visit to `u` ends.  
    - When `u = t`, the current sequence from `s` to `t` is fully represented in `S`.
- Given a pattern sequence `P` of length `K`, verify if `P` is contained in any of the generated sequences.  
  - If yes, `P` is classified as a **K-mer**.
- Use a **rolling hash technique** to efficiently compute the hash values of all length-`K` substrings of the path sequences and compare them with the hash of `P`.

---

## 🧬 GFA Format Overview

The **GFA format** is widely used in bioinformatics to represent the assembly of genomic fragments as graphs.  
This project specifically uses:

- `H (Header)`: Defines the file format.  
- `S (Segment)`: Describes DNA segments (used to build graph nodes).  
- `L (Link)`: Describes links between segments (used to build graph edges).  
- `P (Path)`: Describes full paths through segments (not used here).  
- `W (Walk)`: Describes walks through non-overlapping segments (not used here).

Only `S` and `L` entries are required for this project.

---

## 🧱 Build & Run

This project uses a **Makefile** to simplify compilation and optimization.

### 🛠️ Build

To compile the project in standard mode with debugging symbols and optimizations:

```bash
make compile
```

This will produce an executable named `main` and display the compilation time.

For a faster compilation (without additional flags):

```bash
make fast
```

---

### ▶️ Run

Once compiled, run the program with:

```bash
./main
```

💡 **Tip:** You can edit the `Makefile` to add more build targets (e.g., for testing, profiling, or different datasets).

---

## 🛡️ License

This project is distributed under the [MIT License](./LICENSE).

---

## 🧠 Notes

- All code is implemented in **C++**.
- This project is designed for educational and research purposes in **bioinformatics** and **graph algorithms**.
- Due to potential exponential path growth, pattern search should be performed on small DAGs.
