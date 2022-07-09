#include "strings.h"

String* allocateString(size_t maxLength) {
    String* result = malloc(sizeof(String));
    result->chars = malloc(maxLength * sizeof(char));
    result->length = 0;
    result->maxLength = maxLength;
    return result;
}

void freeString(String* str) {
    free(str->chars);
    free(str);
}