# Original Requirements

Источник: `X:\Projects\BMPEditor\Функциональные требования.docx`, вариант 4.13.
Точный документ доступен в `history/png-image-tool/original-snapshot` и private
архиве. Ниже приведена нормализованная спецификация без служебного текста.

## Общие требования

- Входной формат: PNG, формат необходимо проверять до обработки.
- CLI обязателен, GUI опционален.
- Ввод, вывод и преобразования должны быть отдельными функциями.
- Порядок хранения пикселей должен учитываться.
- Неизменяемые поля PNG-заголовка должны сохраняться.

## Окружность

- `--circle`
- `--center x.y`
- `--radius <positive integer>`
- `--thickness <positive integer>`
- `--color r.g.b`
- необязательные `--fill` и `--fill_color r.g.b`

## Отражение области

- `--mirror`
- `--axis x|y`
- `--left_up left.top`
- `--right_down right.bottom`

## Копирование области

- `--copy`
- исходная область: `--left_up` и `--right_down`
- назначение: `--dest_left_up left.top`
