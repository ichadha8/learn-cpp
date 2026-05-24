#!/usr/bin/env python3
"""Tiny protocol helper for HW05 stretch networking tests."""

import socket
import sys


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: smoke_client.py HOST PORT", file=sys.stderr)
        return 2

    host = sys.argv[1]
    port = int(sys.argv[2])
    with socket.create_connection((host, port), timeout=2.0) as sock:
        sock.sendall(b"JOIN python\n")
        sock.sendall(b"STATE\n")
        data = sock.recv(4096)
        print(data.decode("utf-8", errors="replace"), end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
