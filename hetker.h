#pragma once

#ifndef HETKER_HETKER_H
#define HETKER_HETKER_H

#endif //HETKER_HETKER_H

#include <CL/cl.h>

/**
 * Need to be called to initialize the library on given device
 */
void init();

/**
 * Need to be called to initialize the library on given device
 * @return 0 - device initialized successfully
 *         1 - max number of devices exceeded
 *         <other value> - returns an OpenCL error code that describes an error
 */
cl_int initDevice(cl_device_id device);

cl_device_id* getFullListDevices();
