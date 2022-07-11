#pragma once

#ifndef HETKER_STRINGS_H
#define HETKER_STRINGS_H

#endif //HETKER_STRINGS_H

#include <stdlib.h>

typedef struct tagString String;
struct tagString {
    char* chars;
    size_t length;
    size_t maxLength;
};

String* allocateString(size_t maxLength);

String* constructString(char* chars);

void freeString(String* str);
