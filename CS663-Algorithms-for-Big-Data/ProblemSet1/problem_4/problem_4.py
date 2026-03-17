"""
problem_4.py

Bloom Filter experiments using the PhiUSIIL Phishing URL Dataset
and mmh3 hash functions.

Part 1: Build Bloom filters at target FP rates, run 100k queries, report results.
Part 2: Proof of modulo re-scaling is in the written answer.
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

    print(f"\nPositive queries: {N_POSITIVE:,}")
    print(f"Negative queries: {N_NEGATIVE:,}")
    print()

    results = []

    for fp_rate in TARGET_FP_RATES:
        bf = BloomFilter(n=len(all_urls), fp_rate=fp_rate)

        for url in all_urls:
            bf.add(url)

        # Test positives (should all return True)
        false_negatives = sum(1 for u in positive_urls if not bf.query(u))

        # Test negatives (false positives)
        false_positives = sum(1 for u in negative_urls if bf.query(u))

        empirical_fp = false_positives / N_NEGATIVE

        results.append(
            {
                "target_fp": fp_rate,
                "m_bits": bf.m,
                "k_hashes": bf.k,
                "false_negatives": false_negatives,
                "false_positives": false_positives,
                "empirical_fp": empirical_fp,
            }
        )

        print(
            f"Target FP={fp_rate:.4f} | "
            f"m={bf.m:,} bits | "
            f"k={bf.k} hashes | "
            f"FN={false_negatives} | "
            f"FP={false_positives} | "
            f"Empirical FP={empirical_fp:.6f}"
        )

    # Save results
    out = Path("bloom_results.csv")
    with out.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=results[0].keys())
        writer.writeheader()
        writer.writerows(results)
    print(f"\nResults saved to {out.resolve()}")


if __name__ == "__main__":
    main()
