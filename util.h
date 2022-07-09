#pragma once

#ifndef HETKER_UTIL_H
#define HETKER_UTIL_H

#endif //HETKER_UTIL_H

#include "hetker_lib.h"
#include "strings.h"

typedef struct tagDeviceList DeviceList;
struct DeviceList {
    cl_device_id* data;
    size_t length;
};

String* readAll(FILE *F);
