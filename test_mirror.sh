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
    output_path="$OUTPUT_DIR/${base}_mirror.png"

    echo "🔁 Отражение по оси Y: $filename → $(basename "$output_path")"
    ./image_tool \
        --input "$input_path" \
        --output "$output_path" \
        --mirror \
        --axis y \
        --left_up 50.50 \
        --right_down 150.150
done

echo "🏁 Тест отражения завершён. Проверьте изображения вручную."
