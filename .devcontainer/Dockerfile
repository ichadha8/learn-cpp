FROM mcr.microsoft.com/devcontainers/cpp:1.2.2-debian-12

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       clang \
       clang-tidy \
       cmake \
       cppcheck \
       gdb \
       git \
       lcov \
       make \
       ninja-build \
       pkg-config \
       python3 \
       python3-pip \
       valgrind \
    && rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install --break-system-packages pytest pytest-timeout

WORKDIR /workspaces/learn-cpp
