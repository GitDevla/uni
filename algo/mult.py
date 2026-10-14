import random

from common import benchmark, print_table

def karatsuba_multiply(a: int, b: int, n: int) -> int:
    if n < 10:
        return a * b
    else:
        m = n // 2
        f = 2**m
        a1 = a // f
        a2 = a % f
        b1 = b // f
        b2 = b % f
        a1b1 = karatsuba_multiply(a1, b1, m)
        a2b2 = karatsuba_multiply(a2, b2, m)
        a1a2 = a1 - a2
        b1b2 = b1 - b2
        if a1a2 < 0:
            e1 = -1
        else:
            e1 = 1
        if b1b2 < 0:
            e2 = -1
        else:
            e2 = 1
        m2 = max(m, n - m)
        c = e1 * e2 * karatsuba_multiply(e1 * a1a2, e2 * b1b2, m2)
        return a1b1 * f**2 + (a1b1 + a2b2 - c) * f + a2b2

def native_multiply(a: int, b: int, n: int) -> int:
    return a * b

def make_args(n: int) -> tuple[int, int, int]:
    a = random.randrange(2**n)
    b = random.randrange(2**n)
    return a, b, n


if __name__ == "__main__":
    results = benchmark(
        fns=[native_multiply, karatsuba_multiply],
        make_args=make_args,
        sizes=[16, 32, 64, 128, 256, 512, 1024],
        repeat=1000,
    )
    print_table(results)
