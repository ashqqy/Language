#! /bin/bash
# mlc (My Language Compiler)

ast_file="./tmp/serialized.ast"

mkdir -p ./tmp
./build/frontend.x $1 > ${ast_file}

if [ ! -s ${ast_file} ]
then
    echo "Error: Frontend did not generate AST"
    exit 1
fi

# ./build/backend.x ${ast_file}
