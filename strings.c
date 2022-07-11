#include <string.h>

#include "strings.h"

String* allocateString(size_t maxLength) {
    String* result = malloc(sizeof(String));
    result->chars = malloc(maxLength * sizeof(char));
    memset(result->chars, 0, maxLength * sizeof(char));
    result->length = 0;
    result->maxLength = maxLength;
    return result;
}

String* constructString(char* chars) {
    String* str = allocateString(strlen(chars));
    memcpy(str->chars, chars, str->maxLength);
    str->length = str->maxLength;
    return str;
}

void freeString(String* str) {
    free(str->chars);
    free(str);
}