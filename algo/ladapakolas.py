import random

from common import benchmark, print_table



def make_args(n: int) -> tuple[list[int],int]:
    all_items = []
    MAX = n*10
    for _ in range(n):
        all_items.append(random.randint(1, MAX))
    all_values = sum(all_items)
    largest = max(all_items)
    max_capacity = max(largest, all_values // 3)
    return (all_items, max_capacity)

def naive_bruteforce(items: list[int], capacity:int) -> int:
    stack = [[[i] for i in items]]
    all_partitions = []
    while stack:
        partition = stack.pop()
        all_partitions.append(partition)
        for a in range(len(partition)):
            for b in range(a+1, len(partition)):
                # merge a b
                new_partition = partition[:a] + [partition[a] + partition[b]] + partition[a+1:b] + partition[b+1:]
                stack.append(new_partition)

    opt = len(items)
    for partition in all_partitions:
        valid = True
        for part in partition:
            if sum(part) > capacity:
                valid = False
                break
        if valid:
            opt = min(opt, len(partition))

    return opt

def naive_bruteforce_2(items: list[int], capacity:int) -> int:
    opt = len(items)
    stack = [[[i] for i in items]]
    all_partitions = []
    while stack:
        partition = stack.pop()
        all_partitions.append(partition)
        for a in range(len(partition)):
            for b in range(a+1, len(partition)):
                # merge a b
                new_partition = partition[:a] + [partition[a] + partition[b]] + partition[a+1:b] + partition[b+1:]

                valid = True
                for part in new_partition:
                    if sum(part) > capacity:
                        valid = False
                        break

                if valid:
                    stack.append(new_partition)
                    opt = min(opt, len(new_partition))

    return opt

def next_fit(items: list[int], capacity:int) -> int:
    opt = 0
    current_bin = 0
    for item in items:
        if current_bin + item > capacity:
            opt += 1
            current_bin = item
        else:
            current_bin += item
    if current_bin > 0:
        opt += 1
    return opt

def first_fit(items: list[int], capacity:int) -> int:
    opt = 0
    bins = []
    for item in items:
        placed = False
        for i in range(len(bins)):
            if bins[i] + item <= capacity:
                bins[i] += item
                placed = True
                break
        if not placed:
            bins.append(item)
            opt += 1
    return opt

def best_fit(items: list[int], capacity:int) -> int:
    opt = 0
    bins = []
    for item in items:
        best_bin = -1
        best_space = capacity + 1
        for i in range(len(bins)):
            space = capacity - bins[i]
            if space >= item and space < best_space:
                best_space = space
                best_bin = i
        if best_bin == -1:
            bins.append(item)
            opt += 1
        else:
            bins[best_bin] += item
    return opt

if __name__ == "__main__":
    results = benchmark(
        fns=[next_fit, first_fit, best_fit],
        make_args=make_args,
        sizes=[10, 100, 500, 1000, 2000],
        repeat=10,
    )
    print_table(results)
