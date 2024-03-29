#include "hetker_lib.h"
#include "kernels.h"

void hetkerInit() {
    deviceList.length = 0;
    for (int i = 0; i < HETKER_KERNELS_COUNT; i++) {
        KERNEL_LENGTHS[i] = strlen(KERNELS[i]);
    }
}

void hetkerFree() {
    for (size_t i = 0; i < deviceList.length; i++) {
        Device* device = deviceList.data[i];
        clReleaseCommandQueue(device->queue);
        clReleaseContext(device->context);
        clReleaseDevice(device->id);
    }
    free(deviceList.data);
}

void hetkerEnableLogging() {
    loggingEnabled = 1;
}

Result* hetkerInitDevice(cl_device_id deviceId) {
    if (deviceList.length == CL_MAX_DEVICES) {
        return loggingResult(1, "Can't init new deviceId: maximum number of initialized devices exceed");
    }

    cl_int errCode;

    cl_context context = clCreateContext( NULL,
                                          1,
                                          &deviceId,
                                          NULL, NULL, &errCode);
    if (errCode != CL_SUCCESS) {
        return loggingResult(errCode, "Error on create context");
    }

    cl_command_queue queue = clCreateCommandQueue(context,
                                                  deviceId,
                                                  CL_QUEUE_PROFILING_ENABLE, &errCode);
    if (errCode != CL_SUCCESS) {
        clReleaseContext(context);
        return loggingResult(errCode, "Error on create command queue");
    }

//    cl_program program = clCreateProgramWithSource(context,
//                                                   4,
//                                                   &KERNELS[0],
//                                                   &KERNEL_LENGTHS[0], &errCode);
//
//    if (errCode != CL_SUCCESS) {
//        clReleaseCommandQueue(queue);
//        clReleaseContext(context);
//        return loggingResult(errCode, "Error on create program");
//    }
//
//    char options[100];
////    sprintf(options, "-D BATCH_SIZE=%i", batchSize);
//
//    if ((errCode = clBuildProgram(program, 1, &deviceId, options, NULL, NULL)) != 0) {
//
//        size_t log_size;
//        clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
//
//        char *log = malloc(log_size);
//        clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
//
//        char* text = malloc(log_size + 50 * sizeof(char));
//        sprintf(text, "Error on build program: %i\n%s\n", errCode, log);
//        free(log);
//        Result* res = loggingResult(errCode, text);
//        free(text);
//
//        clReleaseProgram(program);
//        clReleaseCommandQueue(queue);
//        clReleaseContext(context);
//        return res;
//    }

    Device* device = malloc(sizeof(Device));
    device->id = deviceId;
    device->context = context;
    device->queue = queue;

    deviceList.data[deviceList.length] = device;

    return resultCode(0);
}

int deviceComparator(const cl_device_id *d1, const cl_device_id *d2) {
    cl_device_type type1, type2;
    cl_bool mem1, mem2;

    clGetDeviceInfo(*d1, CL_DEVICE_TYPE, sizeof(cl_device_type), &type1, NULL);
    clGetDeviceInfo(*d2, CL_DEVICE_TYPE, sizeof(cl_device_type), &type2, NULL);

    if (type1 == CL_DEVICE_TYPE_CPU) {
        return type2 != CL_DEVICE_TYPE_CPU;
    } else if (type2 == CL_DEVICE_TYPE_CPU) {
        return -1;
    }

    clGetDeviceInfo(*d1, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(cl_bool), &mem1, NULL);
    clGetDeviceInfo(*d2, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(cl_bool), &mem2, NULL);

    if (mem1 == CL_FALSE) {
        return -(mem2 != CL_FALSE);
    } else {
        return mem2 != CL_TRUE;
    }
}

cl_device_id getPreferredDevice(int index) {
    cl_int err;
    cl_uint numPlatforms;

    err = clGetPlatformIDs(0, NULL, &numPlatforms);

    if (checkErr(err, "Unable to get platform count")) {
        return 0;
    }

    //printf("Platforms: %i\n", numPlatforms);

    cl_platform_id platform[numPlatforms];
    err = clGetPlatformIDs(numPlatforms, platform, NULL);

    if (checkErr(err, "Unable to get platforms")) {
        return 0;
    }

//    printf("Detected %d OpenCL platforms:\n", numPlatforms);
//    printf("----------------------------------------\n");

    // calculate general number of devices
    cl_uint deviceCount = 0;
    for (cl_uint i = 0; i < numPlatforms; i++) {
        cl_uint numDevices;
        err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
        if (err != CL_SUCCESS && err != CL_DEVICE_NOT_FOUND) {
            printf("Unable to get GPU count: %d", err);
            return 0;
        }
        deviceCount += numDevices;
        err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
        if (err != CL_SUCCESS && err != CL_DEVICE_NOT_FOUND) {
            printf("Unable to get CPU count: %d", err);
            return 0;
        }
        deviceCount += numDevices;
    }

    if (deviceCount == 0) {
        printf("There are no OpenCL devices available\n");
        return 0;
    }

    cl_device_id deviceIds[deviceCount];
    cl_uint deviceIndex = 0;

    for (cl_uint i = 0; i < numPlatforms; i++) {
        cl_uint numDevices;
        err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);

        if (err == CL_SUCCESS) {
            cl_device_id gpus[numDevices];
            err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_GPU, numDevices, gpus, 0);

            if (err == CL_SUCCESS) {
                for (cl_uint j = 0; j < numDevices; j++) {
                    // add to main device list
                    deviceIds[deviceIndex++] = gpus[j];
                }
            }
        }

        err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);

        if (err == CL_SUCCESS) {
            cl_device_id cpus[numDevices];
            err = clGetDeviceIDs(platform[i], CL_DEVICE_TYPE_CPU, numDevices, cpus, 0);

            if (err == CL_SUCCESS) {
                for (cl_uint j = 0; j < numDevices; j++) {
                    // add to main device list
                    deviceIds[deviceIndex++] = cpus[j];
                }
            }
        }
    }

    qsort(deviceIds, deviceCount, sizeof(cl_device_id), (int (*)(const void *, const void *)) deviceComparator);

    //printf("deviceCount: %i\n", deviceCount);

    if (index < 0 || index >= deviceCount) {
        index = 0;
    }

    cl_device_id device = deviceIds[index];

    printf("----------------------------------------\n");

    // Device name
    size_t deviceNameSize;
    err = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &deviceNameSize);

    if (err != CL_SUCCESS) {
        printf("Can't get device name for device %d. Error code: %d", device, err);
        return 0;
    }

    cl_uchar deviceName[deviceNameSize];
    err = clGetDeviceInfo(device, CL_DEVICE_NAME, deviceNameSize, deviceName, 0);

    if (err == CL_SUCCESS) {
        printf("Running on device: %s\n", deviceName);
    } else {
        printf("Can't get device name for device %d. Error code: %d", device, err);
        return 0;
    }

    printf("----------------------------------------\n");

    return deviceIds[index];
}