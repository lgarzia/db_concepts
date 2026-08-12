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

all: $(TARGET)

$(TARGET): src/hello.c src/main.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(TARGET) $(TEST_TARGET)
	./$(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

$(TEST_TARGET): src/hello.c tests/test_hello.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all test clean run
