#include "list_op.h"

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
