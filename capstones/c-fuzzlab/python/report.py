#!/usr/bin/env python3
"""Create a small Markdown report for a FuzzLab output directory."""

from __future__ import annotations

import pathlib
import sys


def count_files(path: pathlib.Path) -> int:
    if not path.exists():
        return 0
    return sum(1 for item in path.iterdir() if item.is_file())


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: report.py OUT_DIR", file=sys.stderr)
        return 2

    out_dir = pathlib.Path(sys.argv[1])
    crashes = out_dir / "crashes"
    corpus = out_dir / "corpus"

    print("# FuzzLab Report")
    print()
    print(f"- Output directory: `{out_dir}`")
    print(f"- Corpus files: {count_files(corpus)}")
    print(f"- Crash files: {count_files(crashes)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
