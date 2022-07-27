// define: THRESHOLD
// macro args: input type, filter predicate

kernel void filter(global %s* src, global DeviceElementDescriptor* descriptor, global DeviceTaskDescriptor* task) {
    size_t ind = get_global_id(0);
    for (int i = 0; i < THRESHOLD; i++) {
        size_t real_ind = ind * THRESHOLD + i;
        descriptor[real_ind].alive = predicate(src[real_ind]);
    }
}

// TODO Here is a 'compact' version of filter
//// filter predicate returns (1 <=> predicate is true, 0 <=> predicate is false)
//kernel void filter_count(global %s* src, global size_t* count) {
//    size_t ind = get_global_id(0);
//    for (int i = 0; i < THRESHOLD; i++) {
//        count[ind + i] = predicate(src[ind + i]);
//    }
//}
//
//// here an exclusive prefix sum on "count" array should be performed
//
//// there is an additional param - a descriptor that should tell next node the length of the dst array
//kernel void filter_apply(global %s src, global int* count, global int* prefix, global %s dst, global int* descriptor) {
//    size_t ind = get_global_id(0);
//    for (int i = 0; i < THRESHOLD; i++) {
//        if (count[ind + i]) {
//            dst[prefix[ind + i]] = src[ind + i];
//        }
//    }
//}