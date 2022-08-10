#pragma once

#ifndef HETKER_HETKER_H
#define HETKER_HETKER_H

#endif //HETKER_HETKER_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "strings.h"
#include "util.h"

#include "kernels/header.cl"

// TODO Count & check
#define LIB_KERNEL_COUNT 15
#define CL_MAX_DEVICES 100

typedef struct {
    //unsigned char dataType; // 0 - list, 1 - matrix, 2 - image
    cl_program program;
    cl_kernel kernel;
    size_t inputMemorySize;
    size_t outputMemorySize;
    // TODO
} Task;

typedef struct tagTaskSeqNode TaskSeqNode;
struct tagTaskSeqNode {
    Task* task;
    TaskSeqNode* next;
};

typedef struct {
    TaskSeqNode* head;
    const Device* device;
    int valid;
} TaskSeq;

typedef struct {
    String* kernel;
    StringArray types;
    SizeArray typesSizes;
    StringArray includes;
    SizeArray taskLengths;
    int threshold; // TODO
} TaskDescriptor;

typedef struct {
    TaskDescriptor* data;
    size_t curLength;
    size_t length;
    int conserveMemory; // TODO: Unimplemented
} TaskDescriptorSeq;

extern DeviceList deviceList;
extern int loggingEnabled;

cl_device_id getPreferredDevice(int index);

int checkErr(cl_int err, char* errorMsg);

void logger(const char data[]) {
    if (loggingEnabled) {
        printf("[hetker] %s\n", data);
    }
}

