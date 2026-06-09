# Code Samples C

Учебный набор базовых заготовок на C: строки, списки строк, матрицы, конвертация и простые примеры файлового ввода-вывода.

## Сборка

Основной сценарий рассчитан на последнюю установленную Visual Studio/MSBuild. В текущей рабочей среде используется Visual Studio 18 / MSBuild из:

```powershell
C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe
```

Полная проверка:

```powershell
.\scripts\test.ps1
```

Скрипт находит latest Visual Studio через `vswhere` и собирает решение в конфигурации `Debug|x64`.

## GitHub

Целевое имя репозитория: `c-code-samples`.
Topics: `c`, `type-sample`, `learning`.
