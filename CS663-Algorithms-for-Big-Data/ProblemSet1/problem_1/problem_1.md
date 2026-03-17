# Isaia Pacheco

## Problem 1.

Implement the iterative algorithm for estimate pageranks of webpages from stanford.edu.

Use damping factor alpha = 0.85.

Provide your implementation and the estimate for the top 100 pageranks along with their IDs. 

https://snap.stanford.edu/data/web-Stanford.html

### Answer

#### Python Implementation

```python
"""
problem_1.py
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
    """Save top-k RankEntry results to CSV."""
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
```

#### top_100_pagerank.csv

| Rank | Node ID | PageRank Score     |
|-----:|---------|--------------------|
|    1 | 89073   | 0.011302869146     |
|    2 | 226411  | 0.009267836978     |
|    3 | 241454  | 0.008297276547     |
|    4 | 262860  | 0.003023122854     |
|    5 | 134832  | 0.003001279569     |
|    6 | 234704  | 0.002571732824     |
|    7 | 136821  | 0.002453713639     |
|    8 | 68889   | 0.002430790582     |
|    9 | 105607  | 0.002391047035     |
|   10 | 69358   | 0.002364014468     |
|   11 | 67756   | 0.002301000673     |
|   12 | 225872  | 0.002267415922     |
|   13 | 186750  | 0.002232448285     |
|   14 | 272442  | 0.002146547721     |
|   15 | 251796  | 0.002146547721     |
|   16 | 95163   | 0.002146547721     |
|   17 | 119479  | 0.002036248436     |
|   18 | 231363  | 0.002007699884     |
|   19 | 55788   | 0.001957047189     |
|   20 | 167295  | 0.001916247507     |
|   21 | 179645  | 0.001810438922     |
|   22 | 38342   | 0.001738439456     |
|   23 | 117152  | 0.001403613273     |
|   24 | 198090  | 0.001347306862     |
|   25 | 60210   | 0.001309424898     |
|   26 | 235496  | 0.001296691532     |
|   27 | 132695  | 0.001284733045     |
|   28 | 161890  | 0.001209693545     |
|   29 | 112742  | 0.001207922454     |
|   30 | 145892  | 0.001193799520     |
|   31 | 151428  | 0.001189912101     |
|   32 | 81435   | 0.001182020570     |
|   33 | 60440   | 0.001178270849     |
|   34 | 208542  | 0.001172885379     |
|   35 | 91      | 0.001171530173     |
|   36 | 214128  | 0.001168649959     |
|   37 | 258348  | 0.001167904188     |
|   38 | 222873  | 0.001153294737     |
|   39 | 27904   | 0.001143480404     |
|   40 | 272762  | 0.001143467665     |
|   41 | 93778   | 0.001136295325     |
|   42 | 96358   | 0.001132374775     |
|   43 | 181701  | 0.001107923085     |
|   44 | 247241  | 0.001085572673     |
|   45 | 259455  | 0.001085572673     |
|   46 | 34573   | 0.001085554624     |
|   47 | 183004  | 0.001083816918     |
|   48 | 137632  | 0.001083816918     |
|   49 | 221087  | 0.001083816918     |
|   50 | 77999   | 0.001083816918     |
|   51 | 176790  | 0.001083816918     |
|   52 | 17781   | 0.001083816918     |
|   53 | 96745   | 0.001083777237     |
|   54 | 62478   | 0.001072087298     |
|   55 | 120708  | 0.001072087298     |
|   56 | 158568  | 0.001071271800     |
|   57 | 192120  | 0.001062124018     |
|   58 | 227979  | 0.001040556238     |
|   59 | 245659  | 0.001034473770     |
|   60 | 118244  | 0.001004520463     |
|   61 | 18546   | 0.001003684768     |
|   62 | 28600   | 0.001000106079     |
|   63 | 104767  | 0.000997309513     |
|   64 | 13239   | 0.000994448657     |
|   65 | 101161  | 0.000978458661     |
|   66 | 65463   | 0.000966978837     |
|   67 | 32104   | 0.000953514726     |
|   68 | 279368  | 0.000947797383     |
|   69 | 65580   | 0.000865926897     |
|   70 | 185472  | 0.000850750136     |
|   71 | 170452  | 0.000836289730     |
|   72 | 38949   | 0.000807149966     |
|   73 | 93989   | 0.000789653727     |
|   74 | 273989  | 0.000782242570     |
|   75 | 186902  | 0.000779736708     |
|   76 | 84428   | 0.000779410202     |
|   77 | 2260    | 0.000769734384     |
|   78 | 52022   | 0.000759997618     |
|   79 | 205477  | 0.000759943523     |
|   80 | 134375  | 0.000740264371     |
|   81 | 151981  | 0.000731935897     |
|   82 | 49102   | 0.000730940393     |
|   83 | 17567   | 0.000729217119     |
|   84 | 173905  | 0.000720121173     |
|   85 | 19189   | 0.000717922168     |
|   86 | 272961  | 0.000704043158     |
|   87 | 47258   | 0.000703140845     |
|   88 | 89777   | 0.000690855753     |
|   89 | 210162  | 0.000686790481     |
|   90 | 233602  | 0.000686352844     |
|   91 | 247956  | 0.000685921069     |
|   92 | 137798  | 0.000677822303     |
|   93 | 174665  | 0.000661167773     |
|   94 | 84906   | 0.000645142966     |
|   95 | 113673  | 0.000637149839     |
|   96 | 278083  | 0.000634369366     |
|   97 | 208086  | 0.000630344197     |
|   98 | 67503   | 0.000629874068     |
|   99 | 153026  | 0.000598431691     |
|  100 | 36369   | 0.000594913186     |



The nice thing about Misra-Gries algorithm is its ability to merge. In particular, suppose we want to detect elements whose count is more than $\frac{m}{k}$ and use Misra-Gries with $k-1$ counters.

There are two data streams, say from two different servers, and each stream maintains a set of $k-1$ Misra counters (so there are $2(k-1)$ Misra counters in total).

Let $m_1, m_2$ be the length of streams 1 and 2 respectively. We know that the first set contains all elements that occurs more than $\frac{m_1}{k}$ times in stream 1 and the second set contains all elements that occurs more than $\frac{m_2}{k}$ times in stream 2.

Suppose at some point, we want to merge them into $k-1$ Misra counters so that if an element occurs more than $\frac{m_1+m_2}{k}$ times in the combined stream, it must be in one of these merged $k-1$ Misra counters.

---

**For example:**

* **stream 1:** apple, orange, apple, banana, apple, apple, apple
* **stream 2:** orange, orange, orange, mango, mango, citrus, apple, orange, kiwi

Say $k = 5$, then the combined stream is:

```
apple, orange, apple, banana, apple, apple, apple,
orange, orange, orange, mango, mango, citrus, apple, orange, kiwi
```

the combined counters must contains apple and orange as they are heavy hitters.


### Answer

Let the two servers output MG hash maps $H_1, H_2$ (at most $k-1$ keys each).

#### Merge algorithm

1. **Add the summaries**

$$C[x] = H_1[x] + H_2[x] \quad $$

Now $|C| \le 2(k-1)$.

2. **Reduce back to $k-1$ keys**

While $|C| > k-1$:
   - pick any $k$ distinct keys in $C$
   - let $\delta = \min$ of their counts
   - subtract $\delta$ from all those $k$ keys
   - delete zeros

Return $C$ (now has $\le k-1$ counters).

---

#### Why it works

MG correctness comes from the **deleting $k$ distinct items at a time**, each decrement-all step cancels $k$ distinct items, so there can be at most $(m_1+m_2)/k$ cancellations total. Therefore any item with frequency $> (m_1+m_2)/k$ can't be fully canceled, so it must remain as a key after merging.

---

#### Example ($k=5 \Rightarrow 4$ counters)

**Stream 1:** `apple orange apple banana apple apple apple`

No overflow (only 3 distinct items), so:

$$H_1 = \{\text{apple}:5,\ \text{orange}:1,\ \text{banana}:1\}$$

**Stream 2:** `orange orange orange mango mango citrus apple orange kiwi`

Running MG with 4 counters, last `kiwi` triggers decrement-all, so:

$$H_2 = \{\text{orange}:3,\ \text{mango}:1\}$$

**Merge — add the two summaries:**

$$C = \{\text{apple}:5,\ \text{orange}:4,\ \text{banana}:1,\ \text{mango}:1\}$$

Already $4 = k-1$ keys, so no reduce step needed.

Heavy hitter threshold is $(m_1+m_2)/k = 16/5 = 3.2$, so items with $\ge 4$ occurrences must appear — and we indeed keep **apple** and **orange**.