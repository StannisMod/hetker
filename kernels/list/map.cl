// define: THRESHOLD

// macro args: input type, output type, mapping function

kernel void map(global %s* src, global %s* dst) {
    size_t ind = get_global_id(0);
    for (int i = 0; i < THRESHOLD; i++) {
        dst[ind * THRESHOLD + i] = mapping_function(src[ind * THRESHOLD + i]);
    }
}