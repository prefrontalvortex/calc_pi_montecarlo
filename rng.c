//
// Created by mike on 13/12/16.
//

#include <stdlib.h>
#include <stdio.h>
#include "aux.h"
#include "rng.h"

precise_t rand_uniform() {
    return rand_uniform62();
}

precise_t rand_uniform_r(struct drand48_data *rngbuffer) {
    double result;
    drand48_r(rngbuffer, &result);
    return result;
}

precise_t rand_uniform32(long rand_int) {

    return (precise_t)rand_int / (precise_t)RAND_MAX;

}


        precise_t rand_from_file(FILE *randstream) {
    long rand_int;
//    fgets(&rand_int, sizeof(rand_int), randstream);
    fread(&rand_int, sizeof(rand_int), 1, randstream);

    return (precise_t)rand_int / (precise_t)RAND_MAX;

}

long rand_long_from_file( FILE * randstream) {
    long rand_int;
//    fgets(&rand_int, sizeof(rand_int), randstream);
    fread(&rand_int, sizeof(rand_int), 1, randstream);

    return rand_int;

}

precise_t rand_uniform62() {
    unsigned long accu = (unsigned long) rand();
    accu <<= 31;
    accu |= (unsigned long) rand();
    accu &= RAND62_MAX;
    return accu / (precise_t) RAND62_MAX;
}