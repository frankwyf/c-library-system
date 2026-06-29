# C 図書館管理システム

[![CI](https://github.com/frankwyf/c-library-system/actions/workflows/ci.yml/badge.svg)](https://github.com/frankwyf/c-library-system/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](../LICENSE)
[![C Standard](https://img.shields.io/badge/C-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)

**C99** 標準で記述したコンソールベースの図書館管理システムです。  
書籍カタログの管理・貸出・返却・司書管理機能を備え、ユニットテストと GitHub Actions CI/CD を完備しています。

---

## 🌐 ドキュメント

| 言語 | リンク |
|------|--------|
| 🇬🇧 English | [README.md](../README.md) |
| 🇨🇳 中文 | [docs/README_CN.md](README_CN.md) |
| 🇯🇵 日本語 | [docs/README_JP.md](README_JP.md) *（このファイル）* |

---

## 機能一覧

| 機能 | 一般ユーザー | 司書 |
|------|:----------:|:----:|
| 貸出可能な書籍の一覧表示 | ✓ | ✓ |
| 書籍の借用（最大 4 冊） | ✓ | — |
| 書籍の返却 | ✓ | — |
| 借用中書籍の確認 | ✓ | — |
| 全書籍の一覧表示 | — | ✓ |
| 貸出中書籍の一覧表示 | — | ✓ |
| 書籍の追加 | — | ✓ |
| 書籍の削除 | — | ✓ |

---

## クイックスタート

### 必要環境

| ツール | 最低バージョン |
|--------|-------------|
| GCC または Clang | GCC ≥ 7 / Clang ≥ 6 |
| CMake | ≥ 3.14（テスト実行時） |
| Git | 最新の安定版 |

### Make を使用したビルド（Linux / macOS）

```bash
git clone https://github.com/frankwyf/c-library-system.git
cd c-library-system
make
./build/library data/books.txt
```

### CMake を使用したビルド（クロスプラットフォーム）

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Linux / macOS:
./build/library data/books.txt

# Windows (MinGW):
build\library.exe data\books.txt
```

### テストの実行

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## 書籍データファイルの形式

各レコードは 2 行（著者名・書籍タイトル）で構成され、空行で区切られます。

```
Frank Herbert
Dune

Isaac Asimov
I, Robot
```

サンプルカタログ（12 タイトル）は [`data/books.txt`](../data/books.txt) にあります。

---

## プロジェクト構造

```
c-library-system/
├── .github/workflows/ci.yml     GitHub Actions CI/CD
├── src/
│   ├── structures.h             コアデータ構造（Book・User・Library）
│   ├── main.c                   エントリポイント
│   ├── library.[ch]             初期化・ファイル I/O・メイン CLI ループ
│   ├── librarian.[ch]           司書操作（追加 / 削除 / 一覧）
│   ├── user.[ch]                ユーザー操作（借用 / 返却 / 一覧）
│   └── utility.[ch]             入力ユーティリティ
├── tests/
│   ├── test_library.c           ユニットテスト: library モジュール（7 件）
│   ├── test_user.c              ユニットテスト: user モジュール（10 件）
│   └── test_librarian.c         ユニットテスト: librarian モジュール（9 件）
├── data/books.txt               サンプルデータ
├── docs/                        多言語ドキュメント
├── Makefile                     GNU Make ビルド
├── CMakeLists.txt               CMake ビルド + テスト
├── LICENSE                      MIT ライセンス
├── CONTRIBUTING.md              コントリビューションガイド
└── CHANGELOG.md                 変更履歴
```

---

## 設計上のポイント

- **シングルユーザーセッション**：1 回の実行につき 1 人のユーザーがアクティブです。ログアウト時に借用リストはクリアされます。
- **テスト容易性**：すべてのビジネスロジック関数（`user_do_borrow`・`user_do_return`・`librarian_do_add`・`librarian_do_remove`）は非インタラクティブで、Unity フレームワークによるユニットテストが直接可能です。インタラクティブなラッパーが I/O を担当します。
- **エラーハンドリング**：関数は `exit()` を直接呼び出さず、`LIB_OK` / `LIB_ERR` を返します。
- **容量設定**：デフォルトはカタログ 64 冊・借用上限 4 冊。コンパイル時に変更可能です: `-DMAX_BOOKS=128 -DMAX_BORROWED=8`。

---

## コントリビューション

プルリクエスト歓迎です！[CONTRIBUTING.md](../CONTRIBUTING.md) をご確認ください。

## ライセンス

[MIT](../LICENSE) © 2026 frankwyf
