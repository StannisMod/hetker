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

const char FILTER_KERNEL[] = {
#include "kernels/list/filter.cl"
        0};
const char MAP_KERNEL[] = {
#include "kernels/list/map.cl"
        0};
const char PREFIX_KERNEL[] = {
#include "kernels/list/prefix.cl"
        0};
const char REDUCE_KERNEL[] = {
#include "kernels/list/reduce.cl"
        0};

const char HEADER_KERNEL[] = {
#include "kernels/header.cl"
        0};

// TODO Count & check
#define CL_MAX_KERNEL_LENGTH 10000
#define HETKER_KERNELS_COUNT = 7

const char* KERNELS[] = {
        IMAGE_KERNEL,
        MATRIX_KERNEL,
        TEXT_KERNEL,
        FILTER_KERNEL,
        MAP_KERNEL,
        PREFIX_KERNEL,
        REDUCE_KERNEL
};

size_t KERNEL_LENGTHS[];