import copy
import reprlib
import time
from collections.abc import Callable
from typing import Any
from tqdm import tqdm

def measure(fn: Callable, *args: Any) -> tuple[str, int, Any]:
    start = time.perf_counter_ns()
    result = fn(*args)
    return fn.__name__, time.perf_counter_ns() - start, result


def benchmark(
    fns: list[Callable],
    make_args: Callable,
    sizes: list[int],
    repeat: int,
) -> dict[str, dict[int, list[int]]]:
    results = {fn.__name__: {n: [] for n in sizes} for fn in fns}
    for n in tqdm(sizes, desc="Benchmarking"):
        for _ in tqdm(range(repeat), desc=f"n={n}", leave=False):
            args = make_args(n)
            outputs = {}
            for fn in fns:
                fn_args = copy.deepcopy(args)
                name, time_ns, output = measure(fn, *fn_args)
                outputs[name] = output
                results[name][n].append(time_ns)
            first, *rest = outputs.values()
            print(f"n={n}, args={reprlib.repr(args)}, outputs={outputs}")
            # if any(output != first for output in rest):
            #     lines = [f"outputs differ at n={n}, args={reprlib.repr(args)}"]
            #     lines += [f"  {name} -> {reprlib.repr(output)}" for name, output in outputs.items()]
            #     raise AssertionError("\n".join(lines))
    return results


def print_table(results: dict[str, dict[int, list[int]]]) -> None:
    names = list(results)
    sizes = list(results[names[0]])
    print(f"{'n':>8}" + "".join(f"{name:>20}" for name in names))
    for n in sizes:
        cells = (sum(results[name][n]) / len(results[name][n]) / 1000 for name in names)
        print(f"{n:>8}" + "".join(f"{f'{mean_us:.1f} µs':>20}" for mean_us in cells))
