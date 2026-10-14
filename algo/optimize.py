import random

from common import benchmark, print_table



def make_args(n: int) -> tuple[list[int],int]:
    all_items = []
    MAX = n
    for _ in range(n):
        all_items.append((random.randint(1, MAX), random.randint(1, MAX)))
    return (all_items, n//2)

def calc_weight(all_items: list[tuple[int,int]], x: list[int]) -> int:
    return sum(item[0] for item,is_included in zip(all_items, x) if is_included)

def calc_value(all_items: list[tuple[int,int]], x: list[int]) -> int:
    return sum(item[1] for item,is_included in zip(all_items, x) if is_included)

def convert(k:int, length:int) -> list[int]:
    return [int(x) for x in bin(k)[2:].zfill(length)]

def convert_bin_to_int(x:list[int]) -> int:
    return int("".join(str(i) for i in x), 2)

def naive_bruteforce(items: list[tuple[int,int]], capacity:int) -> int:
    opt = 0
    for k in range(2**len(items)):
        x = convert(k, len(items))
        weight = calc_weight(items, x)
        if weight <= capacity:
            value = calc_value(items, x)
            opt = max(opt, value)
    return opt
    

def backtrack(items: list[tuple[int,int]], capacity:int):
    items = sorted(items, key=lambda x: x[0])
    items.append((999999999999, -999999999999))
    opt = 0
    x = [0] *  len(items)
    p = 0

    while True:
        x[p] = 1
        weight = calc_weight(items, x)
        if weight <= capacity:
            value = calc_value(items, x)
            opt = max(opt, value)
            p += 1
        else:
            x[p] = 0
            while x[p] == 0:
                p-= 1
                if p < 0:
                    return opt
            if p < 0:
                return opt
            x[p] = 0
            p += 1
    return opt

def bfs(items: list[tuple[int,int]], capacity:int):
    items = sorted(items, key=lambda x: x[0])
    stack = [1 << i for i in range(len(items))]
    seen = set()
    opt = 0
    while stack:
        x = stack.pop()

        if x in seen:
            continue
        seen.add(x)
        
        current_subset = [item for i, item in enumerate(items) if (x & (1 << i)) > 0]
        weight = sum(item[0] for item in current_subset)
        if weight > capacity:
            continue
        value = sum(item[1] for item in current_subset)
        opt = max(opt, value)

        for i in range(len(items)):
            if (x & (1 << i)) == 0:
                new_subset = x | (1 << i)
                stack.append(new_subset)
    return opt



def greedy(items: list[tuple[int,int]], capacity:int):
    items = sorted(items, key=lambda x: x[1]/x[0], reverse=True)
    opt = 0
    for item in items:
        if item[0] <= capacity:
            opt += item[1]
            capacity -= item[0]

    return opt


if __name__ == "__main__":
    results = benchmark(
        fns=[backtrack, bfs],
        make_args=make_args,
        sizes=[10, 20, 50],
        repeat=10,
    )
    print_table(results)
