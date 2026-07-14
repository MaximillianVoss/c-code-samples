CC ?= cc
NASM ?= nasm

BUILD_DIR := build
CPPFLAGS := -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
LDFLAGS ?= -no-pie
NASMFLAGS ?= -f elf64 -g -F dwarf

ASM_OBJECT := $(BUILD_DIR)/binary_insertion_sort.o
DEMO := $(BUILD_DIR)/autos-sort-demo
TEST := $(BUILD_DIR)/autos-sort-test
REFERENCE_TEST := $(BUILD_DIR)/autos-sort-reference-test

.PHONY: all clean test test-reference

all: $(DEMO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(ASM_OBJECT): src/binary_insertion_sort.asm | $(BUILD_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

$(DEMO): src/demo.c $(ASM_OBJECT) include/vehicle.h
	$(CC) $(CPPFLAGS) $(CFLAGS) src/demo.c $(ASM_OBJECT) $(LDFLAGS) -o $@

$(TEST): tests/sort_test.c src/reference_sort.c $(ASM_OBJECT) include/vehicle.h
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/sort_test.c src/reference_sort.c $(ASM_OBJECT) $(LDFLAGS) -o $@

$(REFERENCE_TEST): tests/sort_test.c src/reference_sort.c include/vehicle.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DSORT_IMPLEMENTATION=insertion_sort_reference tests/sort_test.c src/reference_sort.c -o $@

test: $(TEST)
	./$(TEST)

test-reference: $(REFERENCE_TEST)
	./$(REFERENCE_TEST)

clean:
	rm -rf $(BUILD_DIR)
