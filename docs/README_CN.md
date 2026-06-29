# C 图书馆管理系统

[![CI](https://github.com/frankwyf/c-library-system/actions/workflows/ci.yml/badge.svg)](https://github.com/frankwyf/c-library-system/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](../LICENSE)
[![C Standard](https://img.shields.io/badge/C-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)

基于 **C99** 标准编写的命令行图书馆管理系统。  
支持图书目录管理、借阅/归还，以及馆员管理功能，配有完整的单元测试和 GitHub Actions CI/CD 流程。

---

## 🌐 文档语言

| 语言 | 链接 |
|------|------|
| 🇬🇧 English | [README.md](../README.md) |
| 🇨🇳 中文 | [docs/README_CN.md](README_CN.md) *（当前文件）* |
| 🇯🇵 日本語 | [docs/README_JP.md](README_JP.md) |

---

## 功能概览

| 功能 | 普通用户 | 馆员 |
|------|:-------:|:----:|
| 查看可借图书 | ✓ | ✓ |
| 借阅图书（最多 4 本） | ✓ | — |
| 归还图书 | ✓ | — |
| 查看我的借阅列表 | ✓ | — |
| 查看所有图书 | — | ✓ |
| 查看已借出图书 | — | ✓ |
| 新增图书 | — | ✓ |
| 删除图书 | — | ✓ |

---

## 快速开始

### 环境要求

| 工具 | 最低版本 |
|------|---------|
| GCC 或 Clang | GCC ≥ 7 / Clang ≥ 6 |
| CMake | ≥ 3.14（运行测试时需要） |
| Git | 任意近期版本 |

### 使用 Make 构建（Linux / macOS）

```bash
git clone https://github.com/frankwyf/c-library-system.git
cd c-library-system
make
./build/library data/books.txt
```

### 使用 CMake 构建（跨平台）

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Linux / macOS：
./build/library data/books.txt

# Windows（MinGW）：
build\library.exe data\books.txt
```

### 运行测试

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## 图书数据文件格式

每条记录占两行（作者名、书名），记录之间用空行分隔：

```
Frank Herbert
Dune

Isaac Asimov
I, Robot
```

示例数据文件位于 [`data/books.txt`](../data/books.txt)（共 12 部图书）。

---

## 项目结构

```
c-library-system/
├── .github/workflows/ci.yml     GitHub Actions CI/CD 流程
├── src/
│   ├── structures.h             核心数据结构（Book、User、Library）
│   ├── main.c                   程序入口
│   ├── library.[ch]             初始化、文件读写、主菜单循环
│   ├── librarian.[ch]           馆员操作（新增/删除/列表）
│   ├── user.[ch]                用户操作（借阅/归还/列表）
│   └── utility.[ch]             输入辅助函数
├── tests/
│   ├── test_library.c           单元测试：library 模块（7 个）
│   ├── test_user.c              单元测试：user 模块（10 个）
│   └── test_librarian.c         单元测试：librarian 模块（9 个）
├── data/books.txt               示例图书数据
├── docs/                        多语言文档
├── Makefile                     GNU Make 构建脚本
├── CMakeLists.txt               CMake 构建脚本 + 测试
├── LICENSE                      MIT 许可证
├── CONTRIBUTING.md              贡献指南
└── CHANGELOG.md                 版本历史
```

---

## 设计说明

- **单用户会话**：每次运行只有一个活跃用户，注销时借阅记录清空。
- **可测试性**：所有业务逻辑函数（`user_do_borrow`、`user_do_return`、`librarian_do_add`、`librarian_do_remove`）均为非交互式，可通过 Unity 框架直接进行单元测试；交互式封装层负责 I/O。
- **错误处理**：函数返回 `LIB_OK` / `LIB_ERR`，不直接调用 `exit()`。
- **容量配置**：目录上限默认 64 本，借阅上限默认 4 本；可在编译时覆盖：`-DMAX_BOOKS=128 -DMAX_BORROWED=8`。

---

## 贡献

欢迎提交 PR！请先阅读 [CONTRIBUTING.md](../CONTRIBUTING.md)。

## 许可证

[MIT](../LICENSE) © 2026 frankwyf
