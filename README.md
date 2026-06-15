# Code Samples C

RU: учебный набор базовых заготовок на C: строки, списки строк, матрицы, конвертация и простые примеры файлового ввода-вывода.

EN: a learning-oriented C sample collection with strings, string lists, matrices, conversion helpers, and simple file I/O examples.

## Проверка / Verification

Основной сценарий рассчитан на последнюю установленную Visual Studio/MSBuild. В текущей рабочей среде используется Visual Studio 18 / MSBuild из:

The default workflow uses the latest installed Visual Studio/MSBuild. In the current environment that is Visual Studio 18 / MSBuild from:

```powershell
C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe
```

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
- `docs/refactoring-analysis.md`: краткий анализ найденных проблем и принятых решений.

## GitHub

Целевое имя репозитория / target repository name: `c-code-samples`.

Topics: `c`, `type-sample`, `learning`, `status-active`.
