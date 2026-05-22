# Instructions

This file is the quick-start guide for using the training repo. The assignment
READMEs explain what to implement. This file explains how to set up the
environment, build code, run tests, and avoid common setup problems.

## Recommended Setup

The easiest path is GitHub Codespaces because it uses the repo's devcontainer
configuration automatically.

Recommended order:

1. Use GitHub Codespaces if both learners are comfortable working in the
   browser or VS Code connected to Codespaces.
2. Use VS Code Dev Containers with Docker Desktop if working locally.
3. Use a plain Ubuntu container manually only if you already know Docker well.

The repo is configured for Linux because the assignments use CMake, GCC/Clang,
POSIX APIs, file descriptors, processes, sockets, and pthreads.

## Required Tools

For GitHub Codespaces:

- GitHub account;
- access to this repository;
- browser or VS Code.

For local development:

- Git;
- Docker Desktop;
- VS Code;
- VS Code Dev Containers extension;
- VS Code C/C++ extension pack;
- VS Code CMake Tools extension.

You do not need to install CMake or GCC directly on Windows if you use
Codespaces or Dev Containers. They are installed inside the Linux environment.

## Option A: GitHub Codespaces

1. Open the repository on GitHub.
2. Click `Code`.
3. Open the `Codespaces` tab.
4. Create a new Codespace.
5. Wait for the container to finish building.
6. Open a terminal in Codespaces.

Run:

```bash
cmake --preset dev
cmake --build --preset dev
```

If that works, the environment is ready.

## Option B: Local VS Code Dev Container

1. Install Docker Desktop.
2. Start Docker Desktop and wait until it says it is running.
3. Open the repo folder in VS Code.
4. Install the Dev Containers extension if VS Code asks.
5. Press `Ctrl+Shift+P`.
6. Run `Dev Containers: Reopen in Container`.
7. Wait for the container to build.
8. Open a terminal inside VS Code.

Run:

```bash
cmake --preset dev
cmake --build --preset dev
```

If that works, the environment is ready.

## Important Folder

All course files live inside:

```text
learn-cpp/
```

Run CMake commands from inside `learn-cpp/`, not from the parent folder.

If your terminal is in the parent repo folder, run:

```bash
cd learn-cpp
```

## Build the Whole Starter Scaffold

This checks that the provided starter code compiles.

```bash
cmake --preset dev
cmake --build --preset dev
```

The scaffold build should pass even though many assignment functions are TODO
stubs.

## Run One Assignment

Each assignment has a CMake preset.

Example for HW01:

```bash
cmake --preset hw01
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

Example for the C++ warmup:

```bash
cmake --preset bonus-cpp-warmup
cmake --build --preset bonus-cpp-warmup
ctest --preset bonus-cpp-warmup --output-on-failure
```

The assignment tests are expected to fail at first. That is normal. The tests
describe the behavior you need to implement.

## Preset List

Use these preset names:

```text
dev
hw01
hw02
hw03
bonus-cpp-warmup
hw04
hw05
capstone-c
capstone-cpp
```

What they mean:

| Preset | Project |
|:--|:--|
| `dev` | Build all starter code without assignment tests |
| `hw01` | C puzzle engine |
| `hw02` | Unix program runner |
| `hw03` | fuzzer-lite |
| `bonus-cpp-warmup` | C++ Starship Codex warmup |
| `hw04` | C++ RAII systems library |
| `hw05` | concurrent server core |
| `capstone-c` | FuzzLab |
| `capstone-cpp` | PulseKV |

## Suggested Work Loop

Use this loop for every assignment:

1. Read the lesson.
2. Read the assignment README fully.
3. Configure and build the assignment.
4. Run the tests and observe the first failure.
5. Implement one small function.
6. Rebuild.
7. Re-run tests.
8. Commit when you reach a working milestone.
9. Write a short note about one bug, design choice, or debugging command.

Do not try to implement the entire assignment before running tests. Systems bugs
get much harder when several incomplete pieces are mixed together.

## Where to Edit

Most assignments follow this shape:

```text
assignments/<assignment-name>/
|-- README.md
|-- include/
|-- src/
`-- tests/
```

Usually:

- read `README.md`;
- keep public function signatures in `include/` stable;
- implement TODOs in `src/`;
- read tests in `tests/`;
- add your own tests when useful.

The public headers are part of the assignment contract. Avoid changing them
unless the README explicitly says it is allowed.

## Codespaces vs Docker Desktop

Codespaces and local Dev Containers use the same idea: a Linux development
environment described by `.devcontainer/`.

Codespaces runs that environment in GitHub's cloud.

Local Dev Containers run that environment on your machine through Docker
Desktop.

Either is fine. Codespaces is usually easier for beginners. Local containers are
nice when you want everything on your own machine.

## Git Workflow

Use small commits.

Good rhythm:

```bash
git status
git add <files>
git commit -m "Implement puzzle row validation"
git push
```

Before pushing, run the relevant assignment build and tests.

For example:

```bash
cmake --build --preset hw01
ctest --preset hw01 --output-on-failure
```

## GitHub Actions

The repo has a GitHub Actions workflow in:

```text
.github/workflows/test.yml
```

On push, it builds the starter scaffold.

The full assignment-test matrix is available as a manual workflow. Those tests
may fail while assignments are still incomplete, so use them when you are ready
to check a specific milestone.

## Common Problems

### `cmake: command not found`

You are probably not inside Codespaces or the Dev Container.

Fix:

- reopen the repo in Codespaces; or
- in VS Code, run `Dev Containers: Reopen in Container`.

### Docker Desktop is not running

Start Docker Desktop first. Wait until the engine is fully running, then reopen
the devcontainer.

### Commands are being run from the wrong folder

CMake presets live in `learn-cpp/CMakePresets.json`.

Fix:

```bash
cd learn-cpp
```

Then rerun the command.

### Tests fail immediately

That is normal for assignment presets. The starter code contains TODO stubs.

The `dev` preset should build. Assignment tests should pass only after you
implement the required behavior.

### Git says `index.lock` exists

This usually means a previous Git operation crashed or another Git process is
still running.

First, close editors or terminals that may be running Git. Then check whether a
Git process is still active. Only remove `.git/index.lock` if no Git operation
is running.

### Windows path issues

Prefer working inside Codespaces or the Dev Container terminal. The assignments
target Linux APIs, so Windows PowerShell is useful for Git and file management,
but not for running POSIX systems code directly.

## Debugging Tools

Use these inside the Linux environment:

```bash
gdb ./path/to/test_binary
valgrind ./path/to/test_binary
ctest --preset hw01 --output-on-failure
```

For sanitizer builds, assignment READMEs may include more specific commands.

## Reading Order

Suggested core order:

1. `COURSE.md`
2. `lessons/00-course-map.md`
3. `lessons/01-c-linux-tooling.md`
4. `assignments/hw01-c-puzzle/README.md`
5. Continue week by week through the course plan.

Before C++ RAII, read:

```text
lessons/05b-cpp-warmup.md
assignments/bonus-cpp-warmup/README.md
lessons/05-modern-cpp-core.md
lessons/06-cpp-raii-stl-templates.md
```

## Quick Sanity Check

From `learn-cpp/`, this should work in a correct environment:

```bash
cmake --preset dev
cmake --build --preset dev
```

If it does, you are ready to start the assignments.
