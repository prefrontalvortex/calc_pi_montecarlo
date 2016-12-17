//
// Created by mike on 13/12/16.
//

#ifndef CALC_PI_MC_RNG_H
#define CALC_PI_MC_RNG_H

#include "aux.h"

#define RAND62_MAX 4611686018427387903



typedef struct _RedbearRNG_data {
    uint128_t state;
    uint128_t newstate;
    uint128_t state_xor;
    uint128_t newstate_xor;

    uint128_t output;
    precise_t uniform;
} RedbearRNG_data;

typedef struct _Randobuff {
    uint128_t *states;
    long idx;
    long max;
} Randobuff;

precise_t rand_uniform();
precise_t rand_uniform_r(struct drand48_data *rngbuffer);
precise_t rand_uniform32(long rand_int);
precise_t rand_from_file(FILE *randstream);
long rand_long_from_file( FILE * randstream);
precise_t rand_uniform62();

uint128_t rand_redbear_r(RedbearRNG_data *rngbuffer);
precise_t rand_redbear_uniform_r(RedbearRNG_data *rngbuffer);
uint128_t srand_redbear_r(uint128_t seed, RedbearRNG_data *rngbuffer);



#endif //CALC_PI_MC_RNG_H
