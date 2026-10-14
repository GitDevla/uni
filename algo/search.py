import random

from common import benchmark, print_table


def binary_search(array: list[int], target: int) -> int:
    low, high = 0, len(array) - 1
    while low <= high:
        mid = (low + high) // 2
        if array[mid] == target:
            return mid
        if array[mid] < target:
            low = mid + 1
        else:
            high = mid - 1
    return -1


def linear_search(array: list[int], target: int) -> int:
    i = 0
    while i < len(array) and array[i] < target:
        i += 1
    return i if i < len(array) and array[i] == target else -1

def native(array: list[int], target: int) -> int:
    try:
        return array.index(target)
    except ValueError:
        return -1

def naive(array: list[int], target: int) -> int:
    for i, value in enumerate(array):
        if value == target:
            return i
    return -1


def make_args(n: int) -> tuple[list[int], int]:
    array = sorted(random.sample(range(10 * n), n))
    return array, random.randrange(10 * n)


if __name__ == "__main__":
    results = benchmark(
        fns=[binary_search, linear_search, native, naive],
        make_args=make_args,
        sizes=[100, 1000, 10000],
        repeat=1000,
    )
    print_table(results)
