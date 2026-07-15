# C FileFinder Plugin Lab

RU: восстановленная Linux-лабораторная 2022 года по поиску файлов через динамически загружаемые плагины. Исходные ZIP-этапы сохранены в Git-ветке `history/c-file-finder-2022`:

- `original-c-file-finder-old-2022` — `FileFinder OLD.zip`;
- `original-c-file-finder-current-2022` — `FileFinder.zip`.

EN: a recovered 2022 Linux coursework project for plugin-based file search. The exact source stages are available in the history branch and tags above.

## Refactored core

The portable `byte_frequency` library extracts the part that can be tested independently of `dlopen`, `getopt_long`, and `nftw`:

- strict decimal, binary, and hexadecimal byte parsing;
- a complete 256-entry frequency table, including byte `0xff`;
- deterministic lowest-byte tie handling;
- bounded, streaming file reads with explicit error results.

The original Linux plugin source remains in `legacy/current`. Third-party teaching scaffolding in the original archive retains its original attribution in the history branch.

## Verification

```powershell
.\scripts\test.ps1
```

Original archive hashes and entry manifests are under `preservation`.
