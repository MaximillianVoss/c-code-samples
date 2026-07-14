# ASM/C Binary Insertion Sort

RU: историческая учебная работа 2021 года по вызову NASM-кода из C. Программа сортирует массив 32-байтных записей `Vehicle` бинарными вставками по расчетному запасу хода.

EN: a preserved 2021 exercise demonstrating a C caller linked with a NASM binary-insertion-sort implementation.

## Current sample

- `include/vehicle.h` defines the shared C/assembly ABI and checks the 32-byte record size at compile time.
- `src/demo.c` is a deterministic, leak-free demonstration.
- `src/binary_insertion_sort.asm` retains the original generated System V AMD64 implementation with an ABI note.
- `src/reference_sort.c` provides a readable C reference implementation.
- `tests/sort_test.c` compares the assembly result with the C implementation.

The build targets GNU/Linux x86-64 and requires a C11 compiler, GNU Make, and NASM:

```bash
make
./build/autos-sort-demo
make test
```

`make test-reference` checks the portable C implementation without requiring NASM. `make test` additionally links and checks the preserved assembly implementation.

## Preservation

The original folders were `C/Autos sort` and `Assembler/Autos sort`. Their useful source variants and all three original notes are retained under `preservation/`; `preservation/file-metadata.csv` records every one of the 77 original files with path, SHA-256, attributes, and Windows timestamps.

The union snapshot entered this repository in commit `6269a37ce3f087b088239f4ec261c6a6f8eb6663`, before the working refactor. The complete private snapshot, including binaries and manifest-only build output, is preserved in [`legacy-native-projects-archive`](https://github.com/MaximillianVoss/legacy-native-projects-archive) at commit `54a7410ae04822b5e609059ecaf0f4d56f729b8c`.

Git commit dates describe the import and refactoring work. The metadata CSV is authoritative for the original filesystem dates, while the private archive is authoritative for exact pre-cleanup bytes.

## Scope and limitations

This is an educational ABI/interoperability example, not a portable sorting library. The assembly is tied to the System V AMD64 calling convention and a fixed record layout. The preserved `Backup` variant and unrelated shape-container files are historical evidence only and are not part of the current build.
