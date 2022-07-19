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

typedef struct tagTaskSeqNode TaskSeqNode;
struct tagTaskSeqNode {
    Task* task;
    TaskSeqNode* next;
};

typedef struct tagTaskSeq TaskSeq;
struct tagTaskSeq {
    TaskSeqNode* head;
    Device* device;
};

typedef struct tagTaskDescriptor TaskDescriptor;
struct tagTaskDescriptor {
    const String* kernel;
    size_t typesCount;
    const String* types;
    const String* includes;
    size_t includesLength;
    Device* device;
};

typedef struct tagTaskDescriptorSeq TaskDescriptorSeq;
struct tagTaskDescriptorSeq {
    TaskDescriptor* data;
    size_t curLength;
    size_t length;
};

extern DeviceList deviceList;
extern int loggingEnabled;

cl_device_id getPreferredDevice(int index);

int checkErr(cl_int err, char* errorMsg);

void logger(const char data[]) {
    if (loggingEnabled) {
        printf("[hetker] %s", data);
    }
}

