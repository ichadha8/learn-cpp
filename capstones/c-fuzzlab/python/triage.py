#!/usr/bin/env python3
"""Group FuzzLab crash artifacts by a stable content hash."""

from __future__ import annotations

import hashlib
import pathlib
import sys


def digest(path: pathlib.Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()[:16]


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: triage.py CRASH_DIR", file=sys.stderr)
        return 2

    crash_dir = pathlib.Path(sys.argv[1])
    if not crash_dir.is_dir():
        print(f"not a directory: {crash_dir}", file=sys.stderr)
        return 1

    groups: dict[str, list[pathlib.Path]] = {}
    for path in sorted(crash_dir.glob("*")):
        if path.is_file():
            groups.setdefault(digest(path), []).append(path)

    print("hash,count,examples")
    for key, paths in sorted(groups.items()):
        examples = " ".join(str(p.name) for p in paths[:3])
        print(f"{key},{len(paths)},{examples}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
