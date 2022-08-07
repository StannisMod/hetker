#ifndef HETKER_LIST_OP_H
#define HETKER_LIST_OP_H

#endif //HETKER_LIST_OP_H

#include "hetker_lib.h"

TaskSeq* compileTaskSeq(TaskDescriptorSeq* descriptors, const Device* device);

TaskDescriptorSeq* newTaskDescriptorSeq(size_t seqLength, String* inputType, String* includes, size_t includesLength);

