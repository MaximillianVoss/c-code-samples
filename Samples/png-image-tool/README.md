# PNG Image Tool

Учебный CLI-инструмент на C для чтения и преобразования PNG: окружность,
зеркальное отражение прямоугольной области и копирование области.

Проект восстановлен из каталога `X:\Projects\BMPEditor`. Исходное название
неточно: программа работает с PNG, а не с BMP. Неизменённый source-only снимок
доступен в ветке `history/png-image-tool/original-snapshot` (commit `b780c74`).
Полная исходная папка, включая NTFS-даты, старые бинарники и результаты ручных
тестов, хранится в private-репозитории `legacy-external-projects-archive`.

## Что изменено

- обработка пикселей отделена от PNG I/O и CLI;
- память изображения хранится одним проверяемым блоком;
- RGB и RGBA обрабатываются одинаково, alpha-канал не меняется при рисовании;
- копирование перекрывающихся областей использует временный снимок;
- параметры CLI разбираются строго и одинаково на Windows и Linux;
- добавлены автоматические unit- и PNG round-trip тесты.

## Поддерживаемый PNG

Текущая версия принимает 8-bit RGB и RGBA. Поля IHDR (размер, bit depth,
color type, compression, filter и interlace) сохраняются. Ancillary chunks,
например текстовые комментарии и ICC-профиль, пока не переносятся; это явно
зафиксированное ограничение, а не скрытая потеря данных.

## Проверка

Полная Windows-проверка использует CMake, Ninja и manifest mode актуального
Visual Studio vcpkg. Сборка и зависимости создаются во временном каталоге:

```powershell
.\scripts\test.ps1
```

Проверка только независимого от libpng ядра:

```powershell
.\scripts\test.ps1 -CoreOnly
```

На Linux достаточно установить `cmake`, C-компилятор и development package
`libpng`, затем выполнить стандартный CMake workflow.

## CLI

```text
png-image-tool --input in.png --output out.png --circle \
  --center 100.100 --radius 50 --thickness 4 \
  --color 0.255.0 --fill --fill_color 255.255.0

png-image-tool --input in.png --output out.png --mirror \
  --axis y --left_up 50.50 --right_down 150.150

png-image-tool --input in.png --output out.png --copy \
  --left_up 20.20 --right_down 120.120 --dest_left_up 150.30
```

Координаты прямоугольников включают обе границы. `axis x` меняет местами левую
и правую стороны, `axis y` меняет верх и низ, как в исходной программе.
