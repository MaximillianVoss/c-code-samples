#!/bin/bash
set -e

echo "📦 Компиляция..."
gcc main.c -o image_tool -lpng -lm

INPUT_DIR="images/input"
OUTPUT_DIR="images/output"
mkdir -p "$OUTPUT_DIR"

for input_path in "$INPUT_DIR"/*.png; do
    filename=$(basename "$input_path")
    base="${filename%.*}"
    output_path="$OUTPUT_DIR/${base}_circle.png"

    echo "🧪 Рисуем окружность в $filename → $(basename "$output_path")"
    ./image_tool \
        --input "$input_path" \
        --output "$output_path" \
        --circle \
        --center 100.100 \
        --radius 50 \
        --thickness 4 \
        --color 0.255.0 \
        --fill \
        --fill_color 255.255.0
done

echo "🏁 Тест рисования окружности завершён. Проверьте изображения вручную."
