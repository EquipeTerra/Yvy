#!/bin/bash

# Pega o diretório onde o script está (que é a pasta com o CMakeLists.txt)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p "$SCRIPT_DIR/build"
cd "$SCRIPT_DIR/build"

# Roda cmake apontando para o diretório atual (lib)
cmake "$SCRIPT_DIR"

make -j$(nproc)

if [ -f ./compilated_program ]; then
    ./compilated_program
else
    echo "Executável não encontrado!"
fi
