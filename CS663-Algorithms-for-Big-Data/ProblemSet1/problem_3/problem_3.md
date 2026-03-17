# Isaia Pacheco

## Problem 3.

Consider the following random graph.
There are n vertices labeled from $\{v_{1}, v_{2}, \dots, v_n\}$. 

$\{v_{1}, v_{2}, \dots, v_k\}$ all have directed edges to each other.

Each of the remaining nodes $\{v_{k+1}, v_{k+2}, \dots, v_n\}$, have an edge toward $42$ other nodes chosen at random among $\{v_{1}, v_{2}, \dots, v_n\}$. 

Given the damping factor $d$, compute the expected pagerank of each node. 
Note that you only need to consider two groups 
$\{v_{1}, v_{2}, \dots, v_k\}$ and 
$\{v_{k+1}, v_{k+2}, \dots, v_n\}$ since nodes within each group are identical.

### Answer

**Key observations:**

- All $k$ clique nodes are structurally identical, so they share the same PageRank $PR_c$.
- All $n-k$ remaining nodes are structurally identical, so they share the same PageRank $PR_r$.
- Clique nodes have out-degree $k-1$ (they only link to each other, not to remaining nodes).
- Each remaining node links to 42 nodes chosen uniformly at random from the other $n-1$ nodes, so it sends PageRank to any fixed node with probability $\frac{42}{n-1}$, contributing $\frac{PR_r}{n-1}$ in expectation.

The standard PageRank formula is:

$$PR(v) = \frac{1-d}{n} + d \sum_{u \to v} \frac{PR(u)}{\text{outdeg}(u)}$$

---

#### PageRank of a clique node

A clique node receives from:
- $k-1$ other clique nodes (each with out-degree $k-1$): contribution $= (k-1)\cdot\dfrac{PR_c}{k-1} = PR_c$
- $n-k$ remaining nodes in expectation: contribution $= (n-k)\cdot\dfrac{PR_r}{n-1}$

$$PR_c = \frac{1-d}{n} + d\left[PR_c + \frac{(n-k)\,PR_r}{n-1}\right]$$

Rearranging:

$${(1-d)\,PR_c - \frac{d(n-k)}{n-1}\,PR_r = \frac{1-d}{n}}$$

---

#### PageRank of a remaining node

A remaining node receives from:
- Clique nodes: **0** (clique nodes only link within the clique)
- $n-k-1$ other remaining nodes in expectation: contribution $= (n-k-1)\cdot\dfrac{PR_r}{n-1}$

$$PR_r = \frac{1-d}{n} + d\cdot\frac{(n-k-1)\,PR_r}{n-1}$$

Rearranging:

$${\left(1 - \frac{d(n-k-1)}{n-1}\right)PR_r = \frac{1-d}{n}}$$

---

#### Matrix formulation

$$Ax = b, \qquad x = \begin{bmatrix}PR_c \\ PR_r\end{bmatrix}$$

$$A = \begin{bmatrix} 1-d & -\dfrac{d(n-k)}{n-1} \\[8pt] 0 & 1 - \dfrac{d(n-k-1)}{n-1} \end{bmatrix}, \qquad b = \begin{bmatrix} \dfrac{1-d}{n} \\[8pt] \dfrac{1-d}{n} \end{bmatrix}$$

Since $A$ is upper triangular, we solve directly: first $PR_r$ from row 2, then substitute into row 1 for $PR_c$.

$$PR_r = \frac{(1-d)(n-1)}{n\bigl(n-1-d(n-k-1)\bigr)}$$

$$PR_c = \frac{n-1+d}{n\bigl(n-1-d(n-k-1)\bigr)}$$

---

#### Normalization check

PageRank must sum to 1:

$$k\cdot PR_c + (n-k)\cdot PR_r = 1$$

---

#### Python Implementation

```python
import numpy as np

def solve_pagerank(n, k, d):
    # Construct the 2x2 matrix A
    # Julia's column-major style vs Python's row-major 
    # results in the same structure here:
    A = np.array([
        [1 - d, -d * (n - k) / (n - 1)],
        [0.0,   1 - d * (n - k - 1) / (n - 1)]
    ])
    
    # Construct the vector b
    b = np.array([(1 - d) / n, (1 - d) / n])
    
    # Solve the linear system Ax = b
    x = np.linalg.solve(A, b)
    
    return {"PR_clique": x[0], "PR_remaining": x[1]}

# Execute and print
result = solve_pagerank(100, 10, 0.85)

print(f"Clique node PR:    {result['PR_clique']:.10f}")
print(f"Remaining node PR: {result['PR_remaining']:.10f}")
```