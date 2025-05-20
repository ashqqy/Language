#! /bin/bash
# mlc (My Language Compiler)

ast_file="./tmp/serialized.ast"

mkdir -p ./tmp
./build/frontend.x $1 > ${ast_file}

# ./build/backend.x ${ast_file}
