#!/usr/bin/env python3
"""
Computes measurement errors, mean error, std dev of errors,
and saves a histogram (values shown with 2 decimals).
"""

import re
from statistics import mean, pstdev
import matplotlib.pyplot as plt


def parse_expected(tokens):
    """Convert tokens like '5cm', '50cm', '60' -> floats."""
    out = []
    for t in tokens:
        m = re.match(r"^([-+]?\d*\.?\d+)\s*(cm)?$", t, re.IGNORECASE)
        if not m:
            raise ValueError(f"Can't parse expected token: {t!r}")
        out.append(float(m.group(1)))
    return out


def main():
    expected_tokens = ["5cm", "7cm", "60", "50cm", "9cm",
                       "11cm", "13cm", "15cm", "17cm", "19cm"]
    actual = [6.88, 9.97, 61.41, 51.67, 10.77,
              12.57, 14.57, 16.54, 18.91, 20.70]

    expected = parse_expected(expected_tokens)

    # Error = Actual − Expected
    errors = [a - e for a, e in zip(actual, expected)]

    mu = mean(errors)
    sigma = pstdev(errors)  # population std dev

    # Print rounded results
    print("Expected:", [f"{x:.2f}" for x in expected])
    print("Actual:  ", [f"{x:.2f}" for x in actual])
    print("Errors (Actual - Expected):", [f"{x:.2f}" for x in errors])

    print(f"\nMean of errors: {mu:.2f}")
    print(f"Standard Deviation of errors: {sigma:.2f}")

    # Histogram
    plt.figure(figsize=(7, 4))
    plt.hist(errors, bins="auto", edgecolor="black")
    plt.title("Histogram of Errors (Actual - Expected)")
    plt.xlabel("Error")
    plt.ylabel("Frequency")
    plt.tight_layout()

    out_file = "error_histogram.png"
    plt.savefig(out_file, dpi=200)
    print(f"\nSaved histogram to: {out_file}")

if __name__ == "__main__":
    main()