#pragma once

#ifndef HETKER_HETKER_H
#define HETKER_HETKER_H

#endif //HETKER_HETKER_H

#include <CL/cl.h>

/**
 * Need to be called to initialize the library on given device
 */

void initDevice(cl_device_id device);

cl_device_id* getFullListDevices();
