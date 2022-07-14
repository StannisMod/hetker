#ifndef HETKER_KERNELS_H
#define HETKER_KERNELS_H

#endif //HETKER_KERNELS_H

const char IMAGE_KERNEL[] = {
#include "kernels/image.cl"
        0};
const char MATRIX_KERNEL[] = {
#include "kernels/matrix.cl"
        0};
const char TEXT_KERNEL[] = {
#include "kernels/text.cl"
        0};

// TODO Count & check
#define CL_MAX_KERNEL_LENGTH 10000

const char* KERNELS[] = {
        IMAGE_KERNEL,
        MATRIX_KERNEL,
        TEXT_KERNEL
};

const size_t KERNEL_LENGTHS[4] = {
        CL_MAX_KERNEL_LENGTH,
        CL_MAX_KERNEL_LENGTH,
        CL_MAX_KERNEL_LENGTH
};