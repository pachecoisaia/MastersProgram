"""
problem_1.py

Instructions:
https://snap.stanford.edu/data/web-Stanford.html
Implement the iterative algorithm for estimate pageranks of webpages from stanford.edu.

Use damping factor alpha = 0.85.

Provide your implementation and the estimate for the top 100 pageranks along with their IDs. 
"""

from __future__ import annotations

import gzip
import os
import csv
from pathlib import Path
import urllib.request
from dataclasses import dataclass
from typing import List
import numpy as np

DATA_URL = "https://snap.stanford.edu/data/web-Stanford.txt.gz"
DATA_FILE = "web-Stanford.txt.gz"
OUTPUT_CSV = "top_k_pagerank.csv"

ALPHA = 0.85
TOL = 1e-6
MAX_ITER = 100
GZIP_MODE = "rt"
FILE_ENCODING = "utf-8"
K = 100


@dataclass(frozen=True)
class RankEntry:
    node_id: int
    score: float


@dataclass
class Graph:
    # Mapping between original IDs and dense indices
    nodes: List[int]                # idx -> node_id
    out_degree: np.ndarray          # shape (N,), int32
    inbound: List[List[int]]        # inbound[v_idx] = [u_idx, ...]
    dangling_idx: np.ndarray        # indices where out_degree == 0


def download_dataset(path: str = DATA_FILE) -> str:
    if os.path.exists(path):
        return path
    print(f"Downloading dataset to {path} ...")
    urllib.request.urlretrieve(DATA_URL, path)
    print("Download complete.")
    return path

def read_graph(path: str) -> Graph:
    """
    Build a compact graph representation:
      - nodes: sorted original IDs
      - inbound: inbound adjacency lists in index space
      - out_degree: NumPy array
      - dangling_idx: precomputed
    """
    inbound_by_id: dict[int, list[int]] = {}
    outdeg_by_id: dict[int, int] = {}
    nodes_set: set[int] = set()

    with gzip.open(path, GZIP_MODE, encoding=FILE_ENCODING) as f:
        for line in f:
            if not line or line[0] == "#":
                continue
            u_str, v_str = line.split()
            u = int(u_str)
            v = int(v_str)

            nodes_set.add(u)
            nodes_set.add(v)

            outdeg_by_id[u] = outdeg_by_id.get(u, 0) + 1
            inbound_by_id.setdefault(v, []).append(u)
            inbound_by_id.setdefault(u, [])  # ensure key exists

    nodes = sorted(nodes_set)
    n = len(nodes)
    node_to_idx = {node_id: i for i, node_id in enumerate(nodes)}

    out_degree = np.zeros(n, dtype=np.int32)
    inbound: List[List[int]] = [[] for _ in range(n)]

    for node_id, deg in outdeg_by_id.items():
        out_degree[node_to_idx[node_id]] = deg

    for v_id, src_ids in inbound_by_id.items():
        v_idx = node_to_idx[v_id]
        inbound[v_idx] = [node_to_idx[u_id] for u_id in src_ids]

    dangling_idx = np.where(out_degree == 0)[0]

    return Graph(
        nodes=nodes,
        out_degree=out_degree,
        inbound=inbound,
        dangling_idx=dangling_idx,
    )

def pagerank(graph: Graph, alpha: float = ALPHA, tol: float = TOL, max_iter: int = MAX_ITER) -> np.ndarray:
    """Compute PageRank scores using power iteration."""
    n = len(graph.nodes)
    rank = np.full(n, 1.0 / n, dtype=np.float64)

    for it in range(1, max_iter + 1):
        new_rank = np.zeros(n, dtype=np.float64)

        # inbound contributions
        outdeg = graph.out_degree
        inbound = graph.inbound
        for v in range(n):
            s = 0.0
            for u in inbound[v]:
                deg = outdeg[u]
                if deg > 0:
                    s += rank[u] / deg
            new_rank[v] = s

        dangling_mass = rank[graph.dangling_idx].sum()

        new_rank = alpha * new_rank
        new_rank += (1.0 - alpha) / n
        new_rank += alpha * dangling_mass / n

        l1 = np.abs(new_rank - rank).sum()
        print(f"Iteration {it}: L1 diff={l1:.6e}")

        rank = new_rank
        if l1 < tol:
            print("Converged.")
            break

    return rank

def top_k(graph: Graph, ranks: np.ndarray, k: int = 100) -> List[RankEntry]:
    """Get top-k nodes by PageRank score."""
    idx = np.argsort(-ranks)[:k]
    return [RankEntry(graph.nodes[i], float(ranks[i])) for i in idx]

def save_top_k_csv(top: list[RankEntry], path: str = OUTPUT_CSV) -> None:
    """
    Save top-k RankEntry results to CSV.
    """
    output_path = Path(path)

    with output_path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["rank_position", "node_id", "pagerank_score"])

        for i, entry in enumerate(top, start=1):
            writer.writerow([i, entry.node_id, f"{entry.score:.12f}"])

    print(f"\nSaved CSV to: {output_path.resolve()}")

def print_top_k(top: list[RankEntry]) -> None:
    print(f"\nTop {len(top)} PageRank nodes (node_id, score):")
    for i, entry in enumerate(top, start=1):
        print(f"{i:3d}: {entry.node_id} -> {entry.score:.12f}")


"""
Iterative PageRank

- Downloads dataset if missing
- Builds inbound adjacency (index-based) + out-degree array
- Runs power iteration with alpha=0.85
- Prints top 100 PageRank scores with original node IDs
"""

def main():
    path = download_dataset(DATA_FILE)
    graph = read_graph(path)
    print(f"Nodes: {len(graph.nodes):,}")
    ranks = pagerank(graph, alpha=ALPHA, tol=TOL, max_iter=MAX_ITER)
    top = top_k(graph, ranks, k=K)
    save_top_k_csv(top, path=f"top_{K}_pagerank.csv")
    print_top_k(top)


if __name__ == "__main__":
    main()