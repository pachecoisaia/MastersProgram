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