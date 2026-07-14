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

## Recovered PNG Image Tool

The external folder `X:\Projects\BMPEditor` was a C/libpng PNG command-line
project despite its BMP-oriented name. Its source-only baseline is preserved as
the parentless branch `history/png-image-tool/original-snapshot` at `b780c74`.
The private external-project archive retains all 33 files and their NTFS dates.

The original implementation assumed every decoded row was packed RGB, defined
non-static functions in headers, accepted partially parsed CLI values, omitted
allocation checks, and relied on manual inspection of generated images. The
maintained `Samples/png-image-tool` version separates pixel operations, CLI
parsing, and libpng I/O; supports RGB/RGBA explicitly; validates bounds and
integer ranges; and provides automated core and PNG round-trip tests.

The current PNG layer preserves IHDR fields for supported 8-bit RGB/RGBA input.
Preservation of ancillary chunks is intentionally documented as future work.
