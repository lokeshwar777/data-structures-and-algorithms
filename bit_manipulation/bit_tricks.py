
"""
2's complement
-x = ~x + 1
1. flip all bits
2. add 1 to it
"""


def lsb(n: int):
    """isolate rightmost set bit / lowest set bit / Least Significant Bit"""
    return 0 if n == 0 else n & -n


def highest_power_of_2(x: int):
    """highest power of 2 less than or equal to x"""
    return x.bit_length() - 1


def msb(n: int):
    """isolate leftmost set bit / highest set bit / Most Significant Bit"""
    return 1 << highest_power_of_2(n)


# highest power of 2 less than or equal to x
"""
from math import *
x = 16
print(bin(19))  # 10011

# Rank 1
k = x.bit_length() - 1
print(k)

# Rank 2 (Python only): using math
print(floor(log2(x)))

# Rank 3 (Python only)
LOG = 0
while (1 << (LOG + 1)) <= x:
    LOG += 1
print(LOG, 1 << LOG, bin(LOG))
"""


# isolate leftmost set bit / highest set bit / Most Significant Bit
"""
# faster
def msb(n: int):
    return 1 << (n.bit_length() - 1)
# slower
def msb(n: int):
    while n & (n-1):
        n = n & n - 1
    return n
"""
