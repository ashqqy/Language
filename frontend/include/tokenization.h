#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include <stdlib.h>

#include "ast.h"
#include "token.h"

typedef struct frontend frontend_t;

struct frontend
{
    token_array_t tokens;
    name_table_t  identifiers;
};

void FrontendInit (frontend_t* frontend);
void FrontendDestroy (frontend_t* frontend);

void Tokenization (frontend_t* frontend, char* buffer, size_t buffer_size);

#endif // TOKENIZATION_H
