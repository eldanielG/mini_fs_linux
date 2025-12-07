#!/bin/bash
# Script de compilação para Linux

echo "Compilando MiniFS para Linux..."
gcc -Wall -Wextra -Iinclude -o mini_fs src/main.c src/filesystem.c

if [ $? -eq 0 ]; then
    echo "Compilação concluída com sucesso!"
    echo "Execute com: ./mini_fs"
else
    echo "Erro na compilação!"
    exit 1
fi
