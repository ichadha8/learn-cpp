#!/usr/bin/env python3
"""Convert a PulseKV benchmark text file into a short Markdown report."""

from __future__ import annotations

import pathlib
import sys


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: report.py BENCHMARK_TXT", file=sys.stderr)
        return 2

    path = pathlib.Path(sys.argv[1])
    print("# PulseKV Benchmark Report")
    print()
    for line in path.read_text().splitlines():
        if "=" in line:
            key, value = line.split("=", 1)
            print(f"- {key}: {value}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
