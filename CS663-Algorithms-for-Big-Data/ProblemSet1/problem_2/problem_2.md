# Isaia Pacheco

## Problem 2.

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