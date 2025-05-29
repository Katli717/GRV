#!/bin/bash

# Проверка на количество аргументов
if [ "$#" -ne 2 ]; then
    echo "Использование: $0 <файл> <файл_с_шаблонами>"
    exit 1
fi

input_file="$1"
patterns_file="$2"

# Проверка, существует ли файл
if [ ! -f "$input_file" ]; then
    echo "Файл $input_file не найден!"
    exit 1
fi

# Проверка, существует ли файл с шаблонами
if [ ! -f "$patterns_file" ]; then
    echo "Файл с шаблонами $patterns_file не найден!"
    exit 1
fi

# Используем grep для фильтрации строк
grep -f "$patterns_file" "$input_file" > $input_file.txt

echo "Строки, содержащие шаблоны из '$patterns_file', сохранены в $input_file.txt"
