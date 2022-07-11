#pragma once

#ifndef HETKER_HETKER_H
#define HETKER_HETKER_H

#endif //HETKER_HETKER_H

#include <CL/cl.h>
#include "util.h"

/**
 * Need to be called to initialize the library on given device
 */
void hetkerInit();

void hetkerEnableLogging();

/**
 * Need to be called to initialize the library on given deviceId
 * @return 0 - deviceId initialized successfully
 *         1 - max number of devices exceeded
 *         <other value> - returns an OpenCL error code that describes an error
 */
Result* hetkerInitDevice(cl_device_id deviceId);

cl_device_id* hetkerGetFullListDevices();
