#!/usr/bin/env python3
"""Concurrent PulseKV load generator."""

from __future__ import annotations

import argparse
import concurrent.futures
import socket
import statistics
import time


def run_client(host: str, port: int, ops: int, client_id: int) -> list[float]:
    latencies: list[float] = []
    with socket.create_connection((host, port), timeout=3.0) as sock:
        file = sock.makefile("rwb")
        for i in range(ops):
            key = f"k{client_id}_{i}"
            start = time.perf_counter()
            file.write(f"SET {key} value{i}\n".encode())
            file.flush()
            file.readline()
            file.write(f"GET {key}\n".encode())
            file.flush()
            file.readline()
            latencies.append(time.perf_counter() - start)
        file.write(b"QUIT\n")
        file.flush()
    return latencies


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, required=True)
    parser.add_argument("--clients", type=int, default=4)
    parser.add_argument("--ops", type=int, default=100)
    args = parser.parse_args()

    start = time.perf_counter()
    all_latencies: list[float] = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.clients) as pool:
        futures = [
            pool.submit(run_client, args.host, args.port, args.ops, client_id)
            for client_id in range(args.clients)
        ]
        for future in concurrent.futures.as_completed(futures):
            all_latencies.extend(future.result())

    elapsed = time.perf_counter() - start
    total_ops = args.clients * args.ops * 2
    print(f"clients={args.clients}")
    print(f"ops={total_ops}")
    print(f"elapsed_sec={elapsed:.3f}")
    print(f"throughput_ops_sec={total_ops / elapsed:.1f}")
    if all_latencies:
        print(f"median_roundtrip_ms={statistics.median(all_latencies) * 1000:.3f}")
        print(f"max_roundtrip_ms={max(all_latencies) * 1000:.3f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
