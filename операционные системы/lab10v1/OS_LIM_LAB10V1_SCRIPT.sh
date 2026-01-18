#!/bin/bash
set -e
# Функция проверки наличия пакета libev-dev
check_and_install_libev() 
{
    if dpkg -s libev-dev &>/dev/null; then
        echo "libev-dev уже установлен."
    else
        echo "libev-dev не установлен. Выполняется установка..."
        sudo apt-get update
        sudo apt-get install -y libev-dev
    fi
}
# Проверка и установка libev-dev при необходимости
check_and_install_libev
SOURCE_FILE="OS_LIM_LAB10V1.c"
OUTPUT_FILE="OS_LIM_LAB10V1"
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Исходный файл '$SOURCE_FILE' не найден. Убедитесь, что он находится в текущем каталоге."
    exit 1
fi
gcc -o "$OUTPUT_FILE" "$SOURCE_FILE" -lev
echo "Компиляция завершена."
echo "Запуск программы..."
./"$OUTPUT_FILE"