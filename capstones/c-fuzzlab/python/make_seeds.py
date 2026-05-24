#!/usr/bin/env python3
"""Generate a tiny seed corpus for the demo crashy target."""

from __future__ import annotations

import pathlib
import sys


SEEDS = {
    "seed_000.txt": b"hello\n",
    "seed_001.txt": b"name=world\n",
    "seed_002.txt": b"AAAA\n",
}


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: make_seeds.py SEED_DIR", file=sys.stderr)
        return 2

    seed_dir = pathlib.Path(sys.argv[1])
    seed_dir.mkdir(parents=True, exist_ok=True)
    for name, data in SEEDS.items():
        (seed_dir / name).write_bytes(data)
    print(f"wrote {len(SEEDS)} seeds to {seed_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
