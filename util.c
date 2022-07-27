#include "util.h"

StringArray allocateStringArray(size_t size) {
    StringArray array;
    array.data = malloc(size * sizeof(String));
    array.length = size;
    return array;
}

StringArray wrapStringArray(size_t size, String* data) {
    StringArray array;
    array.data = data;
    array.length = size;
    return array;
}

StringArray createStringArray1(String* first) {
    StringArray array = allocateStringArray(1);
    array.data[0] = *first;
    return array;
}

StringArray createStringArray2(String* first, String* second) {
    StringArray array = allocateStringArray(2);
    array.data[0] = *first;
    array.data[1] = *second;
    return array;
}

StringArray createStringArray3(String* first, String* second, String* third) {
    StringArray array = allocateStringArray(3);
    array.data[0] = *first;
    array.data[1] = *second;
    array.data[2] = *third;
    return array;
}

SizeArray allocateSizeArray(size_t size) {
    SizeArray array;
    array.data = malloc(size * sizeof(String));
    return array;
}

SizeArray wrapSizeArray(size_t size, size_t* data) {
    SizeArray array;
    array.data = data;
    array.length = size;
    return array;
}

SizeArray createSizeArray1(size_t first) {
    SizeArray array = allocateSizeArray(1);
    array.data[0] = first;
    return array;
}

SizeArray createSizeArray2(size_t first, size_t second) {
    SizeArray array = allocateSizeArray(2);
    array.data[0] = first;
    array.data[1] = second;
    return array;
}

SizeArray createSizeArray3(size_t first, size_t second, size_t third) {
    SizeArray array = allocateSizeArray(3);
    array.data[0] = first;
    array.data[1] = second;
    array.data[2] = third;
    return array;
}

Result* resultCode(int code) {
    Result* res = malloc(sizeof(Result));
    res->code = code;
    return res;
}

Result* resultString(int code, String* msg) {
    Result* res = resultCode(code);
    res->message = msg;
    return res;
}

Result* result(int code, char* msg) {
    return resultString(code, constructString(msg));
}

Result* loggingResult(int code, char* msg) {
    logger(msg);
    return result(code, msg);
}

void freeResult(Result* res) {
    freeString(res->message);
}

String* readAll(FILE *F) {
    fseek(F, 0, SEEK_END);
    String* result = allocateString(ftell(F));
    fseek(F, 0, SEEK_SET);

    fread(result->chars, sizeof(char), result->maxLength, F);
    return result;
}
