CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c99 -O2
CPPFLAGS += -Iinclude

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

BIN_DIR := bin
TARGET := $(BIN_DIR)/hello$(EXE)
TEST_TARGET := $(BIN_DIR)/test_hello$(EXE)
CLI_TEST_TARGET := $(BIN_DIR)/test_cli$(EXE)
CONCEPTS_TEST_TARGET := $(BIN_DIR)/test_concepts$(EXE)

all: $(TARGET)

$(TARGET): src/hello.c src/cli.c src/concepts.c src/main.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(TARGET) $(TEST_TARGET) $(CLI_TEST_TARGET) $(CONCEPTS_TEST_TARGET)
	./$(TEST_TARGET)
	./$(CLI_TEST_TARGET)
	./$(CONCEPTS_TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

$(TEST_TARGET): src/hello.c tests/test_hello.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(CLI_TEST_TARGET): src/cli.c tests/test_cli.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(CONCEPTS_TEST_TARGET): src/concepts.c tests/test_concepts.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all test clean run
