# Isaia Pacheco

## Problem 4.

To get started, download the phishing URLs dataset:

* [https://archive.ics.uci.edu/dataset/967/phiusiil+phishing+url+dataset](https://archive.ics.uci.edu/dataset/967/phiusiil+phishing+url+dataset)

Then, take a look at the `mmh3` hash function API (this is one practical choice of hash functions):

* [https://pypi.org/project/mmh3/](https://pypi.org/project/mmh3/)

To create multiple hash functions, the simplest option is to use different seeds.

For example:

```python
import mmh3
h = mmh3.hash128("hello", seed=42)
```

This gives us a random unsigned 128-bit integer. To have hash values between $[0, k-1]$ we can rescale using modulo:

$$i = h \bmod k$$

---

### 1) Bloom Filter Experiments

Generate Bloom filters with various false positive rates **0.1**, **0.01**, **0.001**, and **0.0001** from the given data set.

Make **100,000** lookup queries where:

* **50,000** strings are from the data set, and
* the other **50,000** are random strings that are not in the data set.

Verify that there is **no false negative** and show the **empirical false positive rate** for each case.

Provide your code and a short paragraph reporting the results that you see.


### 2) Justify Modulo Re-scaling

Show that if $h = \texttt{mmh3.hash128}(x)$ is uniformly at random between $[0, N]$ where $N = 2^{128} - 1$, then $h \bmod k$ is near-uniform between $[0, k-1]$. This is to justify the re-scaling we did in part 1.

In particular, suppose $N = qk + r$ where $q$ and $r$ are the quotient and remainder when divided by $k$ (i.e., $q$ and $r$ are integers and $r \in \{0, 1, \ldots, k-1\}$).

Then for a uniformly at random number $h \in [0, N]$, show that:

$$\mathbb{P}(h \bmod k = i) \in \left\{\frac{q}{N+1}, \frac{q+1}{N+1}\right\}$$


---


### Answer

#### Part 1: Bloom Filter Experiments

##### Python Implementation

```python
"""
problem_4.py
"""

from __future__ import annotations

import csv
import math
import os
import random
import string
import zipfile
from pathlib import Path

import mmh3
import urllib.request

DATASET_URL = (
    "https://archive.ics.uci.edu/static/public/967/"
    "phiusiil+phishing+url+dataset.zip"
)
ZIP_FILE = "phiusiil+phishing+url+dataset.zip"
CSV_FILE = "PhiUSIIL_Phishing_URL_Dataset.csv"

TARGET_FP_RATES = [0.1, 0.01, 0.001, 0.0001]
N_POSITIVE = 50_000
N_NEGATIVE = 50_000


class BloomFilter:
    """Bloom filter using mmh3 with different seeds."""

    def __init__(self, n: int, fp_rate: float):
        self.n = n
        self.fp_rate = fp_rate
        # optimal bit array size: m = -n ln(p) / (ln2)^2
        self.m = max(1, int(-n * math.log(fp_rate) / (math.log(2) ** 2)))
        # optimal number of hash functions: k = (m/n) ln2
        self.k = max(1, int((self.m / n) * math.log(2)))
        self.bits = bytearray(self.m)

    def _hashes(self, item: str) -> list[int]:
        return [mmh3.hash128(item, seed=i) % self.m for i in range(self.k)]

    def add(self, item: str) -> None:
        for idx in self._hashes(item):
            self.bits[idx] = 1

    def query(self, item: str) -> bool:
        return all(self.bits[idx] for idx in self._hashes(item))


def download_dataset() -> str:
    if not os.path.exists(CSV_FILE):
        if not os.path.exists(ZIP_FILE):
            print(f"Downloading {ZIP_FILE} ...")
            urllib.request.urlretrieve(DATASET_URL, ZIP_FILE)
        print("Extracting ...")
        with zipfile.ZipFile(ZIP_FILE, "r") as zf:
            zf.extractall(".")
    return CSV_FILE


def load_urls(path: str) -> list[str]:
    urls: list[str] = []
    with open(path, newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            url = row.get("URL") or row.get("url") or ""
            if url:
                urls.append(url)
    return urls


def random_string(length: int = 30) -> str:
    chars = string.ascii_letters + string.digits + "/:.-_~"
    return "".join(random.choices(chars, k=length))


def generate_negative_samples(url_set: set[str], count: int) -> list[str]:
    negatives: list[str] = []
    while len(negatives) < count:
        s = "http://" + random_string(random.randint(15, 60))
        if s not in url_set:
            negatives.append(s)
    return negatives


def main() -> None:
    csv_path = download_dataset()
    all_urls = load_urls(csv_path)
    print(f"Total URLs in dataset: {len(all_urls):,}")

    random.seed(42)
    positive_urls = random.sample(all_urls, N_POSITIVE)
    url_set = set(all_urls)
    negative_urls = generate_negative_samples(url_set, N_NEGATIVE)

    for fp_rate in TARGET_FP_RATES:
        bf = BloomFilter(n=len(all_urls), fp_rate=fp_rate)
        for url in all_urls:
            bf.add(url)

        false_negatives = sum(1 for u in positive_urls if not bf.query(u))
        false_positives = sum(1 for u in negative_urls if bf.query(u))
        empirical_fp = false_positives / N_NEGATIVE

        print(
            f"Target FP={fp_rate:.4f} | "
            f"m={bf.m:,} bits | k={bf.k} hashes | "
            f"FN={false_negatives} | FP={false_positives} | "
            f"Empirical FP={empirical_fp:.6f}"
        )


if __name__ == "__main__":
    main()
```

##### Results

The dataset contains **235,795** URLs.
For each target false positive rate, the Bloom filter was built over all URLs,
then queried with 50,000 in-dataset URLs and 50,000 random strings.

| Target FP Rate | $m$ (bits) | $k$ (hashes) | False Negatives | False Positives | Empirical FP Rate |
|---------------:|-----------:|-------------:|----------------:|----------------:|------------------:|
|         0.1000 |  1,130,054 |            3 |               0 |           5,097 |          0.101940 |
|         0.0100 |  2,260,108 |            6 |               0 |             502 |          0.010040 |
|         0.0010 |  3,390,163 |            9 |               0 |              49 |          0.000980 |
|         0.0001 |  4,520,217 |           13 |               0 |               6 |          0.000120 |

**Observations:**
There are **zero false negatives** in every case, which is expected since a Bloom filter never(very unlikely) produces false negatives by design.
The empirical false positive rates closely match the target rates,
confirming that the optimal parameter formulas $m = -n\ln p / (\ln 2)^2$ and $k = (m/n)\ln 2$ work well in practice.

#### Part 2: Justify Modulo Re-scaling

**Setup:**
Let $h$ be uniform on $\{0, 1, \ldots, N\}$ where $N = 2^{128} - 1$.
There are $N + 1$ equally likely outcomes.
Write $N + 1 = qk + r'$ where $0 \le r' < k$.
(Equivalently, since $N = qk + r$ with $r = N \bmod k$, we have $N+1 = qk + (r+1)$, so $r' = r+1$ unless $r = k-1$ in which case $N+1 = (q+1)k$ and $r' = 0$.)

**Counting the values that map to residue $i$:**

The values $h \in \{0, 1, \ldots, N\}$ with $h \bmod k = i$ are exactly:

$$h \in \{i,\; i+k,\; i+2k,\; \ldots\}$$

The number of such values is $\lfloor (N - i)/k \rfloor + 1$.

- If $i \le r$ (i.e., $i$ is among the "extra" residues), there are $q + 1$ values.
- If $i > r$, there are $q$ values.

(Here $r = N \bmod k$.)

**Computing the probability:**

$$\mathbb{P}(h \bmod k = i) = \frac{\text{count of } h \text{ with } h \bmod k = i}{N+1}$$

So:

$$\mathbb{P}(h \bmod k = i) = \begin{cases} \dfrac{q+1}{N+1} & \text{if } i \le r \\[8pt] \dfrac{q}{N+1} & \text{if } i > r \end{cases}$$

In either case:

$${\mathbb{P}(h \bmod k = i) \in \left\{\frac{q}{N+1},\; \frac{q+1}{N+1}\right\}}$$

**Why this is near-uniform:**
Since $N+1 = 2^{128}$ and $k$ is at most a few million in practice,
$q \approx N/k$ is astronomically large,
so $\frac{q}{N+1} \approx \frac{q+1}{N+1} \approx \frac{1}{k}$.
The maximum deviation from uniformity is $\frac{1}{N+1}$, which is negligible.