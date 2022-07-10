#include "hetker_lib.h"

// TODO Count & check
#define CL_MAX_KERNEL_LENGTH 10000

const unsigned char IMAGE_KERNEL[] = {
#include "kernels/image.cl"
0};
const unsigned char LIST_KERNEL[] = {
#include "kernels/list.cl"
0};
const unsigned char MATRIX_KERNEL[] = {
#include "kernels/matrix.cl"
0};
const unsigned char TEXT_KERNEL[] = {
#include "kernels/text.cl"
0};

const unsigned char* KERNELS[] = {
        IMAGE_KERNEL,
        LIST_KERNEL,
        MATRIX_KERNEL,
        TEXT_KERNEL
};

const size_t KERNEL_LENGTHS[] = {
        CL_MAX_KERNEL_LENGTH,
        CL_MAX_KERNEL_LENGTH,
        CL_MAX_KERNEL_LENGTH,
        CL_MAX_KERNEL_LENGTH
};

void init() {
    deviceList.length = 0;
}

cl_int initDevice(cl_device_id device) {
    if (deviceList.length == CL_MAX_DEVICES) {
        logger("Can't init new device: maximum number of initialized devices exceed");
        return CL_BUILD_ERROR;
    }

    cl_int errCode;

    cl_context context = clCreateContext( NULL,
                                          1,
                                          &device,
                                          NULL, NULL, &errCode);
    if (checkErr(errCode, "Error on create context")) {
        return errCode;
    }

    cl_command_queue queue = clCreateCommandQueue(context,
                                                  device,
                                                  CL_QUEUE_PROFILING_ENABLE, &errCode);
    if (checkErr(errCode, "Error on create command queue")) {
        clReleaseContext(context);
        return errCode;
    }

    // 4. Perform runtime source compilation, and obtain kernelAggregate entry point.
    cl_program program = clCreateProgramWithSource(context,
                                                   1,
                                                   &KERNELS,
                                                   &KERNEL_LENGTHS, &errCode);

    if (checkErr(errCode, "Error on create program")) {
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return 1;
    }

    char options[100];
//    sprintf(options, "-D BATCH_SIZE=%i", batchSize);

    if ((errCode = clBuildProgram(program, 1, &device, options, NULL, NULL)) != 0) {
        printf("Error on build program: %i\n", errCode);

        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        char *log = malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        printf("%s\n", log);
        free(log);

        clReleaseProgram(program);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return 1;
    }

    return 0;
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