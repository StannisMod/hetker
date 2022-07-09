#include "util.h"

String* readAll(FILE *F) {
    fseek(F, 0, SEEK_END);
    String* result = allocateString(ftell(F));
    fseek(F, 0, SEEK_SET);
    result->chars = malloc(result->maxLength * sizeof(char));

    fread(result->chars, sizeof(char), result->maxLength, F);
    return result;
}


