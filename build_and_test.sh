#!/bin/bash

clear

set -e  # Остановить выполнение при ошибке

echo "🔍 Проверка необходимых инструментов..."

# Проверка наличия gcc
if ! command -v gcc >/dev/null 2>&1; then
    echo "❌ gcc не установлен. Установите его: sudo apt install gcc"
    exit 1
fi

# Проверка наличия libpng-dev
if ! dpkg -s libpng-dev >/dev/null 2>&1; then
    echo "❌ libpng-dev не установлен. Установите его: sudo apt install libpng-dev"
    exit 1
fi

echo "✅ Все зависимости найдены."

# Компиляция
echo "🔧 Компиляция..."
gcc main.c -o image_tool -lpng -lm
echo "✅ Скомпилировано: ./image_tool"

# Проверка наличия тестового PNG-файла
TEST_PNG="testImage-1.png"
if [ ! -f "$TEST_PNG" ]; then
    echo "⚠️ Файл $TEST_PNG не найден. Создайте PNG-файл для теста и положите рядом со скриптом."
    exit 1
fi

# Тестовый запуск
echo "🚀 Тестовый запуск:"
./image_tool --input "$TEST_PNG" --info
