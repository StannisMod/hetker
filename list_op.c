#include "list_op.h"
#include "kernels.h"

Result* source(TaskDescriptorSeq* seq, String* outputType, size_t outputTypeSize, void* data, size_t length) {
    TaskDescriptor descriptor;
    descriptor.kernel = constructString("source");
    descriptor.types = createStringArray1(outputType);
    descriptor.typesSizes = createSizeArray1(outputTypeSize);
    descriptor.taskLengths = createSizeArray2(0, 0);

    seq->length++;
    seq->data[seq->length] = descriptor;
    return resultCode(0);
}

Result* filter(TaskDescriptorSeq* seq, String* outputType, size_t outputTypeSize, String* filterFunc) {
    if (seq->curLength == seq->length) {
        return loggingResult(-1, "Sequence length exceeded");
    }
    TaskDescriptor descriptor;
    descriptor.kernel = constructString("filter");
    descriptor.types = createStringArray1(outputType);
    descriptor.includes = createStringArray1(filterFunc);
    descriptor.typesSizes = createSizeArray1(outputTypeSize);
    descriptor.taskLengths = createSizeArray2(0, 0);

    seq->length++;
    seq->data[seq->length] = descriptor;
    return resultCode(0);
}

Result* map(TaskDescriptorSeq* seq, String* outputType, size_t outputTypeSize, String* mapFunc) {
    if (seq->curLength == seq->length) {
        return loggingResult(-1, "Sequence length exceeded");
    }
    TaskDescriptor descriptor;
    descriptor.kernel = constructString("map");
    descriptor.types = createStringArray1(outputType);
    descriptor.includes = createStringArray1(mapFunc);
    descriptor.typesSizes = createSizeArray1(outputTypeSize);
    descriptor.taskLengths = createSizeArray2(0, 0);

    seq->length++;
    seq->data[seq->length] = descriptor;
    return resultCode(0);
}

TaskDescriptorSeq* newTaskDescriptorSeq(size_t seqLength, String* inputType, String* includes, size_t includesLength) {
    TaskDescriptorSeq* res = malloc(sizeof(TaskDescriptorSeq));
    res->data = malloc(seqLength * sizeof(TaskDescriptorSeq));
    res->length = seqLength;
    res->curLength = 0;
    return res;
}

const char* getKernelByName(const char* name) {
    if (!strcmp(name, "filter")) {
        return FILTER_KERNEL;
    } else if (!strcmp(name, "map")) {
        return MAP_KERNEL;
    } else if (!strcmp(name, "prefix")) {
        return PREFIX_KERNEL;
    } else if (!strcmp(name, "reduce")) {
        return REDUCE_KERNEL;
    } else {
        return NULL;
    }
}

TaskSeq* compileTaskSeq(TaskDescriptorSeq* descriptors, const Device* device) {
    TaskSeq* seq = malloc(sizeof(TaskSeq));
    seq->device = device;
    seq->valid = 0;
    Task cur;

    String* nextInputType = NULL;
    String* nextOutputType = NULL;
    for (int i = 0; i < descriptors->curLength; i++) {
        TaskDescriptor* descriptor = &descriptors->data[i];
        nextOutputType = &descriptor->types.data[0];

        const char** includes = malloc((descriptor->includes.length + 2) * sizeof(char *));
        size_t* includesLengths = malloc((descriptor->includes.length + 2) * sizeof(size_t));
        includes[0] = HEADER_KERNEL;
        includes[1] = getKernelByName(descriptor->kernel->chars);
        if (includes[1] == NULL) {
            printf("Can't compile sequence: Kernel %s not recognized", descriptor->kernel->chars);
            return NULL;
        }
        includesLengths[0] = strlen(includes[0]);
        includesLengths[1] = strlen(includes[1]);
        for (int j = 0; j < descriptor->includes.length; j++) {
            includes       [j + 2] = descriptor->includes.data[j].chars;
            includesLengths[j + 2] = descriptor->includes.data[j].length;
        }

        int errCode;
        cl_program program = clCreateProgramWithSource(device->context, 1, includes, includesLengths, &errCode);
        if (checkErr(errCode, "Can't compile sequence: Failed to create program")) {
            return seq;
        }
        free(includes);
        free(includesLengths);

        char options[1000];

        char* inputType = "";
        if (nextInputType != NULL) {
            inputType = nextInputType->chars;
        }
        sprintf(options, "-D THRESHOLD=%i -D INPUT_TYPE=%s -D OUTPUT_TYPE=%s", descriptor->threshold, inputType, nextOutputType->chars);

        if ((errCode = clBuildProgram(program, 1, &device->id, options, NULL, NULL)) != 0) {
            printf("Can't compile sequence: Error on build program: %i\n", errCode);

            size_t log_size;
            clGetProgramBuildInfo(program, device->id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

            char *log = malloc(log_size);
            clGetProgramBuildInfo(program, device->id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

            printf("%s\n", log);
            free(log);

            clReleaseProgram(program);
            return NULL;
        }

        cl_kernel kernel = clCreateKernel(program, descriptor->kernel->chars, &errCode);
        if (checkErr(errCode, "Can't compile sequence: Failed to create kernel")) {
            clReleaseProgram(program);
            return NULL;
        }

        cur.program = program;
        cur.kernel = kernel;
        if (nextInputType != NULL) {
            cur.inputMemorySize = descriptor->taskLengths.data[0] * descriptor->typesSizes.data[0];
        }
        cur.outputMemorySize = descriptor->taskLengths.data[1] * descriptor->typesSizes.data[1];

        nextInputType = nextOutputType;
    }

    seq->valid = 1;
    return seq;
}

void freeTaskDescriptorSeq(TaskDescriptorSeq* descriptors) {
    for (int i = 0; i < descriptors->curLength; i++) {
        TaskDescriptor* descriptor = &descriptors->data[i];
        freeSizeArray(descriptor->taskLengths);
        freeSizeArray(descriptor->typesSizes);
        freeStringArray(descriptor->includes);
        freeStringArray(descriptor->types);
        freeString(descriptor->kernel);
        free(descriptor);
    }
    free(descriptors);
}

Result* runTaskSeq(TaskSeq* seq) {

}