#!/usr/bin/env bash
set -euo pipefail

presets=(hw01 hw02 hw03 hw04 hw05 capstone-c capstone-cpp)

for preset in "${presets[@]}"; do
  cmake --preset "$preset"
  cmake --build --preset "$preset"
  ctest --preset "$preset" --output-on-failure
done
