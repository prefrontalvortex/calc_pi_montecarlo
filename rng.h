//
// Created by mike on 13/12/16.
//

#ifndef CALC_PI_MC_RNG_H
#define CALC_PI_MC_RNG_H

#define RAND62_MAX 4611686018427387903

precise_t rand_uniform();
precise_t rand_uniform_r(struct drand48_data *rngbuffer);
precise_t rand_uniform32(long rand_int);
precise_t rand_from_file(FILE *randstream);
long rand_long_from_file( FILE * randstream);
precise_t rand_uniform62();

#endif //CALC_PI_MC_RNG_H
