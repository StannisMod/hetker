#pragma once

#ifndef HETKER_HETKER_H
#define HETKER_HETKER_H

#endif //HETKER_HETKER_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "strings.h"
#include "util.h"

typedef struct tagTask Task;
struct tagTask {
    unsigned char dataType; // 0 - list, 1 - matrix, 2 - image
    cl_device_id device;
    cl_kernel kernel;
};

extern DeviceList deviceList;

cl_device_id getPreferredDevice(int index);

int checkErr(cl_int err, char* errorMsg);


