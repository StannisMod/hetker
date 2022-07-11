#include "util.h"

String* readAll(FILE *F) {
    fseek(F, 0, SEEK_END);
    String* result = allocateString(ftell(F));
    fseek(F, 0, SEEK_SET);

    fread(result->chars, sizeof(char), result->maxLength, F);
    return result;
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


