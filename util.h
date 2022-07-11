#pragma once

#ifndef HETKER_UTIL_H
#define HETKER_UTIL_H

#endif //HETKER_UTIL_H

#include "hetker_lib.h"
#include "strings.h"

typedef struct tagDevice Device;
struct tagDevice {
    cl_device_id id;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernels[LIB_KERNEL_COUNT];
};

typedef struct tagDeviceList DeviceList;
struct tagDeviceList {
    size_t length;
    Device* data[CL_MAX_DEVICES];
};

typedef struct tagResult Result;
struct tagResult {
    int code;
    String* message;
};

String* readAll(FILE *F);

Result* resultCode(int code);

Result* resultString(int code, String* msg);

Result* result(int code, char* msg);

Result* loggingResult(int code, char* msg);
