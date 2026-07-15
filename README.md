# Code Samples C

RU: учебный набор базовых заготовок на C: строки, списки строк, матрицы, конвертация и простые примеры файлового ввода-вывода.

EN: a learning-oriented C sample collection with strings, string lists, matrices, conversion helpers, and simple file I/O examples.

## Проверка / Verification

Основной сценарий рассчитан на последнюю установленную Visual Studio/MSBuild. В текущей рабочей среде используется Visual Studio 18 / MSBuild из:

The default workflow uses the latest installed Visual Studio/MSBuild. In the current environment that is Visual Studio 18 / MSBuild from:

```powershell
C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe
```

Both native projects use the Visual Studio 2026 `v145` platform toolset and the latest installed Windows 10 SDK.

Полная проверка / full verification:

```powershell
.\scripts\test.ps1
```

Скрипт находит latest Visual Studio через `vswhere`, собирает решение в конфигурации `Debug|x64`, запускает демонстрационный exe и отдельный unit-test runner.

The script resolves the latest Visual Studio through `vswhere`, builds `Debug|x64`, and runs both the demo executable and the dedicated unit-test runner.

## Состав / Contents

- `String.h`, `StringList.h`, `StrHelper.h`: базовые операции со строками и списками строк.
- `MatrixInt.h`: матрицы `int`, транспонирование и умножение.
- `Converter.h`: простые heap-строки из числовых значений.
- `FileIO.h`: path-based примеры текстового и бинарного ввода-вывода на локальных файлах.
- `Code samples C.c`: демонстрационная точка входа и простые self-checks.
- `Code samples C.Tests`: отдельный console runner с unit-тестами для helper-заголовков.
- `Legacy/asm-c-binary-insertion-sort`: документированный пример вызова NASM-сортировки из C с сохраненным исходным снимком.
- `Legacy/c-file-finder-plugin`: восстановленная Linux-лабораторная динамических плагинов и переносимое протестированное ядро подсчета байтов.
- `Legacy/c-array-exercises-2016`: две восстановленные лабораторные по динамическим массивам и матрицам с безопасным C17-ядром и тестами.
- `Samples/png-image-tool`: восстановленный и переработанный PNG CLI с libpng, CMake и автоматическими тестами.
- `docs/refactoring-analysis.md`: краткий анализ найденных проблем и принятых решений.

Исходное состояние найденного `BMPEditor` сохранено отдельно в
`history/png-image-tool/original-snapshot`; полная папка с исходными NTFS-датами
доступна в private-архиве внешних проектов.

## GitHub

Целевое имя репозитория / target repository name: `c-code-samples`.

Topics: `c`, `type-sample`, `learning`, `status-active`.
