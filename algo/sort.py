import random

from common import benchmark, print_table


def make_args(n: int) -> tuple[list[int]]:
    return (random.sample(range(10 * n), n),)

def native(array: list[int]) -> list[int]:
    return sorted(array)

def naive(array: list[int]) -> list[int]:
    for i in range(len(array)):
        for j in range(i + 1, len(array)):
            if array[i] > array[j]:
                array[i], array[j] = array[j], array[i]
    return array


def quick_sort(array: list[int]) -> list[int]:
    if len(array) < 2:
        return array
    pivot = array[0]
    left = [x for x in array[1:] if x <= pivot]
    right = [x for x in array[1:] if x > pivot]
    return quick_sort(left) + [pivot] + quick_sort(right)


def quick_sort_v2(array: list[int]) -> list[int]:
    def qs_helper(array: list[int], lower: int, upper: int) -> None:
        if lower >= upper:
            return

        pivot = array[(lower + upper) // 2]

        i = lower
        j = upper

        while i <= j:
            while array[i] < pivot:
                i += 1

            while array[j] > pivot:
                j -= 1

            if i <= j:
                array[i], array[j] = array[j], array[i]
                i += 1
                j -= 1

        qs_helper(array, lower, j)
        qs_helper(array, i, upper)

    qs_helper(array, 0, len(array) - 1)
    return array


if __name__ == "__main__":
    results = benchmark(
        fns=[native, naive, quick_sort, quick_sort_v2],
        make_args=make_args,
        sizes=[10, 100, 1000],
        repeat=100,
    )
    print_table(results)
