//
// Created by Quarter on 14.07.2022.
//

#include "list_op.h"

Result* filter(TaskDescriptorSeq* seq, const String* outputType, const String* filterFunc) {
    if (seq->curLength == seq->length) {
        return loggingResult(-1, "Sequence length exceeded");
    }
    TaskDescriptor descriptor;
    descriptor.kernel = constructString("filter");
    descriptor.types = outputType;
    descriptor.typesCount = 1;
    descriptor.includes = filterFunc;
    descriptor.includesLength = 1;

    seq->length++;
    seq->data[seq->length] = descriptor;
    return resultCode(0);
}

Result* map(TaskDescriptorSeq* seq, const String* outputType, const String* mapFunc) {
    if (seq->curLength == seq->length) {
        return loggingResult(-1, "Sequence length exceeded");
    }
    TaskDescriptor descriptor;
    descriptor.kernel = constructString("map");
    descriptor.types = outputType;
    descriptor.typesCount = 1;
    descriptor.includes = mapFunc;
    descriptor.includesLength = 1;

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
