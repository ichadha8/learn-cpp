#!/usr/bin/env python3
"""Small PulseKV client for manual and integration testing."""

from __future__ import annotations

import argparse
import socket


class PulseClient:
    def __init__(self, host: str, port: int) -> None:
        self.sock = socket.create_connection((host, port), timeout=3.0)
        self.file = self.sock.makefile("rwb")

    def command(self, line: str) -> str:
        self.file.write(line.encode("utf-8") + b"\n")
        self.file.flush()
        return self.file.readline().decode("utf-8").rstrip("\n")

    def close(self) -> None:
        try:
            self.command("QUIT")
        finally:
            self.file.close()
            self.sock.close()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, required=True)
    parser.add_argument("commands", nargs="*")
    args = parser.parse_args()

    client = PulseClient(args.host, args.port)
    try:
        for command in args.commands:
            print(client.command(command))
    finally:
        client.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
