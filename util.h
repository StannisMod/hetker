#pragma once

#ifndef HETKER_UTIL_H
#define HETKER_UTIL_H

#endif //HETKER_UTIL_H

#include "hetker_lib.h"
#include "strings.h"

typedef struct {
    cl_device_id id;
    cl_context context;
    cl_command_queue queue;
} Device;

typedef struct {
    size_t length;
    Device* data[CL_MAX_DEVICES];
} DeviceList;

typedef struct {
    String* data;
    size_t length;
} StringArray;

StringArray allocateStringArray(size_t size);
StringArray wrapStringArray(size_t size, String* data);
StringArray createStringArray1(String* first);
StringArray createStringArray2(String* first, String* second);
StringArray createStringArray3(String* first, String* second, String* third);
void freeStringArray(StringArray array);

typedef struct {
    size_t* data;
    size_t length;
} SizeArray;

SizeArray allocateSizeArray(size_t size);
SizeArray createSizeArray(size_t size, size_t* data);
SizeArray createSizeArray1(size_t first);
SizeArray createSizeArray2(size_t first, size_t second);
SizeArray createSizeArray3(size_t first, size_t second, size_t third);
void freeSizeArray(SizeArray array);

typedef struct {
    int code;
    String* message;
} Result;

Result* resultCode(int code);

Result* resultString(int code, String* msg);

Result* result(int code, char* msg);

Result* loggingResult(int code, char* msg);

String* readAll(FILE *F);
