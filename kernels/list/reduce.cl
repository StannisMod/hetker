// define: THRESHOLD
// macro args: input type, accumulator type, output type, reducer function, aggregator function

kernel void reduce(global %s* src, global %s* aggregate, global %s* dst) {
    for (int i = 0; i < THRESHOLD; i++) {
        // TODO
    }
}