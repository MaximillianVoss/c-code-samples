#!/bin/bash
set -e

clear
echo "📦 Компиляция..."

gcc main.c -o image_tool -lpng -lm
gcc compare_pixels.c -o compare_pixels -lpng -lm

INPUT_DIR="images/input"
OUTPUT_DIR="images/output"
mkdir -p "$OUTPUT_DIR"

echo "🔍 Ищем PNG-файлы в '$INPUT_DIR'..."
shopt -s nullglob
png_files=("$INPUT_DIR"/*.png)

if [ ${#png_files[@]} -eq 0 ]; then
    echo "⚠️ Нет PNG-файлов в '$INPUT_DIR'."
    exit 1
fi

for input_path in "${png_files[@]}"; do
    filename=$(basename "$input_path")
    base="${filename%.*}"
    output_path="$OUTPUT_DIR/${base}_save_load.png"

    echo "🧪 Сохранение: $filename → $(basename "$output_path")"
    ./image_tool --input "$input_path" --output "$output_path"

    echo "🔬 Сравнение пикселей:"
    if ./compare_pixels "$input_path" "$output_path"; then
        echo "🟢 OK: $filename"
    else
        echo "🔴 FAIL: $filename"
    fi

    echo "-----------------------------"
done

echo "🏁 Тест завершён."
