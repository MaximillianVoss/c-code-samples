# Refactoring Analysis

## Scope

RU: анализ выполнен для учебного набора header-only C helpers: строки, списки строк, матрицы, конвертация и файловый ввод-вывод.

EN: this analysis covers the learning-oriented header-only C helpers: strings, string lists, matrices, conversion, and file I/O.

## Findings

- Header-only functions previously had external linkage. If the same helper header was included by multiple `.c` files in one project, the linker could receive duplicate symbols.
- Ownership rules were implicit: conversion helpers returned heap strings, `StringListAdd` accepted owning `String` values, and callers had to infer cleanup requirements from implementation details.
- `StringIsNumber` rejected useful decimal forms such as `0.5` because it treated any leading `0` in a multi-character string as invalid.
- Matrix allocation failure paths were not explicit enough. A partially allocated matrix could expose non-empty dimensions even when row allocation failed.
- File I/O examples used fixed filenames only. That made the helpers harder to test and less reusable.

## Refactoring Decisions

- Helper functions are now `static` in headers, so each translation unit gets internal-linkage copies and linker conflicts are avoided.
- Memory ownership is documented in code comments near the functions that allocate or take ownership.
- Matrix constructors return an empty `0x0` matrix when allocation fails or dimensions are empty.
- Matrix operations accept `const MatrixInt*` where they only read data.
- File I/O now has path-based helpers, while the original sample wrappers remain for compatibility.
- Unit tests cover string parsing, string list behavior, converters, string append helpers, matrix transpose/multiply, incompatible matrix dimensions, and path-based file I/O.

## Remaining Possible Work

- Split header-only helpers into `.h` declarations and `.c` implementations if the project grows beyond a compact learning sample.
- Add CI after the repository cleanup pass settles, using the same `scripts/test.ps1` entrypoint.
- Rename legacy compatibility functions with typos in a breaking-change pass, after all old samples that may call them are checked.
