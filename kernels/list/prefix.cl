// define: THRESHOLD, INPUT, OUTPUT
// macro args: aggregate_function(OUTPUT, INPUT), reduce_function(OUTPUT, OUTPUT)

typedef struct {
    OUTPUT aggregate;
    OUTPUT inclusivePrefix;
} PARTITION;

kernel void aggregate(const global INPUT* src, global OUTPUT* dst, global PARTITION* p,
                      global DeviceElementDescriptor* descriptor, global DeviceTaskDescriptor* task) {
    size_t ind = get_global_id(0);
    size_t part_start = ind * THRESHOLD;

// 1 & 2 done on host
// 3

    global PARTITION* me = &p[ind];
    // TODO Replace NANs with something acceptable for structures
    me->aggregate = NAN;
    me->inclusivePrefix = NAN;

    OUTPUT sum = src[part_start];
    for (size_t i = part_start + 1; i < part_start + BATCH_SIZE; i++) {
        sum = aggregate_function(sum, src[i]);
    }

    me->aggregate = sum;
    if (ind == 0) {
        me->inclusivePrefix = sum;
    }
}

kernel void reduce(const global INPUT* src, global OUTPUT* dst, global PARTITION* p,
                   global DeviceElementDescriptor* descriptor, global DeviceTaskDescriptor* task) {
    size_t ind = get_global_id(0);
    size_t part_start = ind * BATCH_SIZE;
// 4 & 5
    global PARTITION* me = &p[ind];

    float aggregate = 0;
    for (size_t i = ind; i >= 1; i--) {
        global PARTITION* it = &p[i - 1];

        if (!isnan(it->inclusivePrefix)) {
            aggregate = reduce_function(aggregate, it->inclusivePrefix);
            me->inclusivePrefix = reduce_function(aggregate, me->aggregate);
            break;
        } else {
            aggregate = reduce_function(aggregate, it->aggregate);
        }
    }

    OUTPUT res = aggregate_function(res, src[part_start]);
    dst[part_start] = res;
    for (size_t i = part_start + 1; i < part_start + BATCH_SIZE; i++) {
        res = aggregate_function(res, src[i]);
        dst[i] = reduce_function(res, aggregate);
    }
}