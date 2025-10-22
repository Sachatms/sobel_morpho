# Lab 2 Overview - Git & GitHub Actions Quick Reference

**Course:** QLOG-5EII
**Student:** Sachatms
**Repository:** [sobel_morpho](https://github.com/Sachatms/sobel_morpho)
**Branch:** `workspace_sacha`
**Date:** October 2025

---

## Table of Contents

- [Lab 2 Overview - Git \& GitHub Actions Quick Reference](#lab-2-overview---git--github-actions-quick-reference)
  - [Table of Contents](#table-of-contents)
  - [1. Setup: Fork \& Clone](#1-setup-fork--clone)
    - [What Was Done:](#what-was-done)
    - [Key Files:](#key-files)
  - [2. Part 1: Cherry-Pick \& Conflict Resolution](#2-part-1-cherry-pick--conflict-resolution)
    - [What Was Done:](#what-was-done-1)
    - [Key Learning:](#key-learning)
  - [3. Part 2: Sobel Modification \& Pull Request](#3-part-2-sobel-modification--pull-request)
    - [What Was Done:](#what-was-done-2)
    - [Implementation:](#implementation)
  - [4. Part 3: Pre-Commit Hook for Formatting](#4-part-3-pre-commit-hook-for-formatting)
    - [What Was Done:](#what-was-done-3)
    - [Hook Features:](#hook-features)
  - [5. Part 4: GitHub Actions CI/CD](#5-part-4-github-actions-cicd)
    - [What Was Done:](#what-was-done-4)
    - [Workflows Trigger On:](#workflows-trigger-on)
    - [Key Patterns:](#key-patterns)
  - [6. Part 5: Unit Testing with CMocka](#6-part-5-unit-testing-with-cmocka)
    - [What Was Done:](#what-was-done-5)
    - [Test Results:](#test-results)
  - [Command Reference](#command-reference)
    - [Git Commands](#git-commands)
      - [Setup \& Configuration](#setup--configuration)
      - [Cherry-Picking](#cherry-picking)
      - [Standard Workflow](#standard-workflow)
      - [Viewing Diffs \& Changes](#viewing-diffs--changes)
    - [CMake/Build Commands](#cmakebuild-commands)
    - [GitHub CLI Commands](#github-cli-commands)
    - [Testing Commands](#testing-commands)
    - [Code Formatting](#code-formatting)
    - [GitHub Actions (Local Testing)](#github-actions-local-testing)
  - [Quick Tips](#quick-tips)
    - [When Cherry-Picking Fails:](#when-cherry-picking-fails)
    - [When Pre-Commit Hook Rejects:](#when-pre-commit-hook-rejects)
    - [When GitHub Actions Fail:](#when-github-actions-fail)
    - [When Tests Fail:](#when-tests-fail)
  - [Resources \& Links](#resources--links)

---

## 1. Setup: Fork & Clone

**Objective:** Fork the upstream repository and set up local workspace.

### What Was Done:
- Forked `QLOG-5EII/sobel_morpho` to `Sachatms/sobel_morpho`
- Cloned locally into `lab2_git/` directory
- Created working branch `workspace_sacha`
- Configured remotes: `origin` (fork) and `upstream` (original)

### Key Files:
- C-based image processing application
- Sobel/Dilation/Erosion filters for video processing
- CMake build system with SDL2 dependencies

---

## 2. Part 1: Cherry-Pick & Conflict Resolution

**Objective:** Copy a specific commit from `other-branch` and resolve conflicts.

### What Was Done:
1. **Found commit** `cc48bf8` on `upstream/other-branch` (message: "temp")
2. **Cherry-picked** → Merge conflict in `src/sobel.c`
3. **Resolved conflict:** Kept proper Sobel formula with `sqrt(gx² + gy²)`
4. **Renamed commit:** "fix: Implement proper Sobel magnitude calculation with sqrt"
5. **Final commit:** `269dde0`

### Key Learning:
- Cherry-pick preserves original author attribution
- Conflict resolution requires understanding both versions
- `git commit --amend` for renaming commits

---

## 3. Part 2: Sobel Modification & Pull Request

**Objective:** Replace Sobel edge detection with a mean/average filter.

### What Was Done:
1. **Modified** `src/sobel.c`: Replaced gradient calculation with 3×3 mean filter
2. **Committed:** "feat: Replace Sobel edge detection with mean averaging filter" (`c001053`)
3. **Pushed** to `origin/workspace_sacha`
4. **Created PR #14** to `QLOG-5EII/sobel_morpho` using GitHub CLI

### Implementation:
```c
// Sum all 9 pixels in 3x3 neighborhood
int sum = /* top-left + top-center + ... + bottom-right */;
output[(j-1)*width + i] = sum / 9;  // Average
```

---

## 4. Part 3: Pre-Commit Hook for Formatting

**Objective:** Enforce code formatting using `clang-format` before commits.

### What Was Done:
1. **Created** `.githooks/pre-commit` bash script
2. **Validates** all staged C/C++ files with `clang-format`
3. **Rejects commits** if formatting is incorrect
4. **Versioned** in `.githooks/` directory (not `.git/hooks/`)
5. **Team setup:** `git config core.hooksPath .githooks`

### Hook Features:
- Uses `#!/usr/bin/env bash` for NixOS compatibility
- Checks only staged files (`.c`, `.cpp`, `.h`, `.hpp`)
- Provides helpful error messages with fix commands
- Returns proper exit codes (0 = success, 1 = error)

---

## 5. Part 4: GitHub Actions CI/CD

**Objective:** Automate builds on Ubuntu and Windows for every push.

### What Was Done:
1. **Created 3 workflows:**
   - `ubuntu-ci.yml` - Ubuntu-only build
   - `multi-os-discrete.yml` - Separate jobs for Ubuntu/Windows
   - `multi-os-matrix.yml` - Matrix strategy (recommended)

2. **Fixed path issue:** Verification step was looking for `build/sobel` from root, but executable was in `build/` directory. Added `working-directory: build` to fix.

### Workflows Trigger On:
- Push to `main` or `workspace_sacha`
- Pull requests to `main`
- Manual dispatch (`workflow_dispatch`)

### Key Patterns:
- **Matrix strategy** = Less code duplication, easy to scale
- **Discrete jobs** = More flexibility, different steps per OS
- **Conditional steps** = `if: matrix.os == 'ubuntu-latest'`

---

## 6. Part 5: Unit Testing with CMocka

**Objective:** Add unit tests and integrate into CI/CD pipeline.

### What Was Done:
1. **Created** `tests/test_sobel.c` with 5 test cases:
   - ✅ Uniform image test
   - ✅ Gradient smoothing test
   - ✅ Edge handling test
   - ✅ Zero value boundary test
   - ✅ Max value (255) boundary test

2. **Configured CMake:**
   - `tests/CMakeLists.txt` - Test build config
   - Updated root `CMakeLists.txt` with `BUILD_TESTING` option

3. **Updated workflows:**
   - Added `libcmocka-dev` to Ubuntu jobs
   - Added CMocka build from source for Windows
   - Added `ctest --output-on-failure` step

4. **Added to dev environment:** `cmocka` in `flake.nix`

### Test Results:
```
[==========] tests: 5 test(s) run.
[  PASSED  ] 5 test(s).
```

---

## Command Reference

### Git Commands

#### Setup & Configuration
```bash
# Fork and clone
gh repo fork QLOG-5EII/sobel_morpho --clone=true --remote=true lab2_git

# Create working branch
git checkout -b workspace_sacha

# View branches
git branch -a

# Configure hooks path
git config core.hooksPath .githooks
```

#### Cherry-Picking
```bash
# View commits on another branch
git log upstream/other-branch --oneline

# Cherry-pick a commit
git cherry-pick <commit-hash>

# Resolve conflicts
git add <conflicted-file>
git cherry-pick --continue

# Abort cherry-pick
git cherry-pick --abort

# Rename last commit
git commit --amend -m "new message"
```

#### Standard Workflow
```bash
# Check status
git status

# Stage files
git add <file>
git add .

# Commit
git commit -m "message"

# Push to remote
git push origin <branch>

# View commit history
git log --oneline --graph --all
```

#### Viewing Diffs & Changes
```bash
# View changes
git diff

# View staged changes
git diff --cached

# View commit details
git show <commit-hash>
```

### CMake/Build Commands

```bash
# Configure build (no tests)
mkdir build && cd build
cmake ..

# Configure build WITH tests
cmake .. -DBUILD_TESTING=ON

# Build project
make

# Build in release mode
cmake --build . --config Release

# Clean build
rm -rf build
```

### GitHub CLI Commands

```bash
# Create pull request
gh pr create \
  --base main \
  --head Sachatms:workspace_sacha \
  --title "Title" \
  --body "Description" \
  --repo QLOG-5EII/sobel_morpho

# View PR status
gh pr status

# List PRs
gh pr list

# View PR details
gh pr view <number>
```

### Testing Commands

```bash
# Run all tests with CTest
cd build
ctest

# Run tests with verbose output on failure
ctest --output-on-failure

# Run tests with verbose output always
ctest --verbose

# Run specific test
ctest -R SobelTests

# Run test executable directly
./Release/test_sobel
./Debug/test_sobel
```

### Code Formatting

```bash
# Format a single file
clang-format -i src/sobel.c

# Format all C/C++ files
clang-format -i src/*.c src/*.h

# Check format without modifying (dry-run)
clang-format --dry-run src/sobel.c

# View formatting differences
clang-format src/sobel.c | diff - src/sobel.c
```

### GitHub Actions (Local Testing)

```bash
# View workflow runs
gh workflow list

# View specific workflow runs
gh run list --workflow=ubuntu-ci.yml

# View run details
gh run view <run-id>

# Re-run a workflow
gh run rerun <run-id>

# Watch a running workflow
gh run watch
```

---

## Quick Tips

### When Cherry-Picking Fails:
1. Check conflict markers: `<<<<<<<`, `=======`, `>>>>>>>`
2. Choose correct version (or merge manually)
3. Remove conflict markers
4. `git add <file>` → `git cherry-pick --continue`

### When Pre-Commit Hook Rejects:
1. Format files: `clang-format -i <file>`
2. Re-stage: `git add <file>`
3. Try commit again

### When GitHub Actions Fail:
1. Check logs in Actions tab
2. Look for path issues (`working-directory`)
3. Verify dependencies are installed
4. Check if executables are in expected location

### When Tests Fail:
1. Run locally first: `cd build && ctest --output-on-failure`
2. Run test directly: `./Release/test_sobel`
3. Check test logic in `tests/test_sobel.c`
4. Verify CMakeLists.txt has correct include paths

---

## Resources & Links

- **Fork:** https://github.com/Sachatms/sobel_morpho
- **Upstream:** https://github.com/QLOG-5EII/sobel_morpho
- **PR #14:** https://github.com/QLOG-5EII/sobel_morpho/pull/14
- **CMocka Docs:** https://cmocka.org/
- **GitHub Actions Docs:** https://docs.github.com/en/actions

