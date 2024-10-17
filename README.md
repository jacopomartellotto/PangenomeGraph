# Pangenome Graph Construction and Analysis Project

The project focuses on the construction and analysis of a pangenome graph, which allows for representing and analyzing genetic variations within a set of genomes in bioinformatics. Using the GFA (Graphical Fragment Assembly) format, the project involves reading GFA files and creating a labeled graph, as well as performing various operations on it.

## Project Objectives

### I. Graph Reading and Construction

- Read a GFA file and create a labeled, directed graph **G**, using adjacency lists (vector in C++).
- Label the nodes and edges of **G** based on the information provided in the GFA file.

### II. Graph Analysis

- Verify if the graph **G** is cyclic: if it is, perform a DFS traversal and remove back edges so that **G** becomes a DAG (Directed Acyclic Graph).
- Consider a source **s** (in-degree zero) and a destination **t** (out-degree zero). If there are multiple sources or destinations, select a specific one for the analysis.

### III. Pattern Search on DAG Paths

- Perform a depth-first search (DFS) on the DAG **G** starting from the source **s** to find all paths from **s** to **t**.
  - Note that the number of such paths can be exponential, so only verify this on a small DAG.
  - Ignore the boolean "visited" array since the graph is acyclic, and use a character array **S** as auxiliary storage: modify the recursive DFS traversal seen in class, adding and removing the string of the current node **u** when **u** is first visited and when the visit to **u** ends, respectively. (Note: when **u = t**, the sequence corresponding to the current path from **s** to **t** is in **S**.)
- Given a pattern sequence **P** of length **K**, verify if it is contained in one of the sequences generated as described above. If so, **P** is called a **K-mer**.
- Use rolling hash techniques to calculate the hash of the length **K** portions in the sequence (as covered in class) and compare them with the hash of pattern **P**.

## GFA Structure

The GFA format represents the graphical assembly of genomic fragments. Each line in the GFA file has a specific field:

- **H (Header)**: The first line indicating the type of format.
- **S (Segment)**: Lines that describe DNA segments.
- **L (Link)**: Lines that describe links between segments.
- **P (Path)**: Lines that describe paths through segments.
- **W (Walk)**: Lines that describe paths through non-overlapping segments.

Only use **S** for nodes and **L** for edges.
