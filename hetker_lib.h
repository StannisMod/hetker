#pragma once

#ifndef HETKER_HETKER_H
#define HETKER_HETKER_H

#endif //HETKER_HETKER_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "strings.h"
#include "util.h"

// TODO Count & check
#define LIB_KERNEL_COUNT 15
#define CL_MAX_DEVICES 100

typedef struct tagTask Task;
struct tagTask {
    unsigned char dataType; // 0 - list, 1 - matrix, 2 - image
    cl_device_id device;
    cl_kernel kernel;
};

extern DeviceList deviceList;

cl_device_id getPreferredDevice(int index);

int checkErr(cl_int err, char* errorMsg);

void logger(const char data[]) {
    printf("[hetker] %s", data);
}

