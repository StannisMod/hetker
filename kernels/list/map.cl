// define: THRESHOLD

// macro args: input type, output type, mapping function

kernel void map(global %s* src, global %s* dst, global DeviceElementDescriptor* descriptor, global DeviceTaskDescriptor* task) {
    size_t ind = get_global_id(0);
    for (int i = 0; i < THRESHOLD; i++) {
        size_t real_ind = ind * THRESHOLD + i;
        if (descriptor[real_ind].alive) {
            dst[real_ind] = mapping_function(src[real_ind]);
        }
    }
}