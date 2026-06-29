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

.PHONY: all clean run help

all: $(BUILD_DIR) $(EXE)

$(BUILD_DIR):
	mkdir -p $@

$(EXE): $(SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $^ $(LDFLAGS) -o $@

run: all
	./$(EXE) $(DATA_DIR)/books.txt

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "Targets:"
	@echo "  all    Build the library executable (default)"
	@echo "  run    Build and run with sample data"
	@echo "  clean  Remove build artefacts"
	@echo ""
	@echo "For unit tests:"
	@echo "  cmake -B build && cmake --build build && ctest --test-dir build"
