#!/bin/bash
set -e

echo "📦 Компиляция..."
gcc main.c -o image_tool -lpng -lm

INPUT_FILE="images/input/testImage-3.png"
OUTPUT_FILE="images/output/testImage-3_copy_custom.png"

echo "📋 Копируем область: 97.129 – 202.236 → 296.172"
./image_tool \
    --input "$INPUT_FILE" \
    --output "$OUTPUT_FILE" \
    --copy \
    --left_up 97.129 \
    --right_down 202.236 \
    --dest_left_up 296.172

echo "✅ Готово. Проверь результат: $OUTPUT_FILE"
