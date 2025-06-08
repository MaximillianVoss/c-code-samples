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
    output_path="$OUTPUT_DIR/${base}_copy.png"

    echo "📋 Копирование области: $filename → $(basename "$output_path")"
    ./image_tool \
        --input "$input_path" \
        --output "$output_path" \
        --copy \
        --left_up 20.20 \
        --right_down 120.120 \
        --dest_left_up 150.30
done

echo "🏁 Тест копирования завершён. Проверьте изображения вручную."
