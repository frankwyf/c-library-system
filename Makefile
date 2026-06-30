# C Library Management System
# GNU Makefile — Linux / macOS development convenience build.
# For cross-platform builds and unit tests, use CMakeLists.txt.

CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -Wpedantic -g
LDFLAGS :=

SRC_DIR   := src
BUILD_DIR := build
DATA_DIR  := data

SRCS := $(SRC_DIR)/main.c       \
        $(SRC_DIR)/library.c    \
        $(SRC_DIR)/librarian.c  \
        $(SRC_DIR)/user.c       \
        $(SRC_DIR)/utility.c

EXE := $(BUILD_DIR)/library

# Source files that clang-format should check
FMT_SRCS := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.h tests/*.c)

.PHONY: all clean run test coverage format format-check help

all: $(BUILD_DIR) $(EXE)

$(BUILD_DIR):
	mkdir -p $@

$(EXE): $(SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $^ $(LDFLAGS) -o $@

run: all
	./$(EXE) $(DATA_DIR)/books.txt

# Build and run all unit tests via CMake + CTest
test:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
	cmake --build $(BUILD_DIR) --parallel
	ctest --test-dir $(BUILD_DIR) --output-on-failure -V

# Build with gcov instrumentation and produce an lcov HTML report
coverage:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug \
	      -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON
	cmake --build $(BUILD_DIR) --parallel
	ctest --test-dir $(BUILD_DIR) --output-on-failure
	lcov --capture --directory $(BUILD_DIR) --output-file coverage.info \
	     --ignore-errors inconsistent,inconsistent
	lcov --remove coverage.info '*/unity/*' '*/tests/*' '/usr/*' \
	     --output-file coverage.info --ignore-errors unused
	genhtml coverage.info --output-directory coverage-html
	@echo "Coverage report: coverage-html/index.html"

# Apply clang-format to all C source and header files
format:
	clang-format -i $(FMT_SRCS)

# Dry-run clang-format check (non-zero exit if any file would change)
format-check:
	clang-format --dry-run --Werror $(FMT_SRCS)

clean:
	rm -rf $(BUILD_DIR) coverage.info coverage-html

help:
	@echo "Targets:"
	@echo "  all           Build the library executable (default)"
	@echo "  run           Build and run with sample data"
	@echo "  test          Build and run all unit tests (requires CMake)"
	@echo "  coverage      Generate lcov HTML coverage report"
	@echo "  format        Auto-format all sources with clang-format"
	@echo "  format-check  Verify formatting without modifying files"
	@echo "  clean         Remove build artefacts"
