// define: THRESHOLD, INPUT, OUTPUT

// filter predicate returns (1 <=> predicate is true, 0 <=> predicate is false)
kernel void compact_count(global INPUT* src, global size_t* count) {
    size_t ind = get_global_id(0);
    for (int i = 0; i < THRESHOLD; i++) {
        count[ind + i] = predicate(src[ind + i]);
    }
}

// here an inclusive prefix sum on "count" array should be performed

// there is an additional param - a descriptor that should tell next node the length of the dst array
kernel void compact_apply(global INPUT* src, global size_t* prefix, global OUTPUT* dst, global DeviceElementDescriptor* descriptor, global DeviceTaskDescriptor* task) {
    size_t ind = get_global_id(0);
    for (int i = 0; i < THRESHOLD; i++) {
        size_t real_ind = ind * THRESHOLD + i;
        if (descriptor[real_ind].alive) {
            dst[prefix[real_ind]] = src[real_ind];
        }
    }

    if (ind == get_global_size(0) - 1) {
        task->real_size = prefix[ind];
    }
}