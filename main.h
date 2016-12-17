//
// Created by mm on 12/16/16.
//

#ifndef CALC_PI_MC_MAIN_H
#define CALC_PI_MC_MAIN_H

#include "aux.h"
#include "rng.h"

precise_t eps_to_alpha(precise_t epsilon);
precise_t estimate_pi3(struct drand48_data *rngbuf, long batch_size);
precise_t estimate_pi3_special(RedbearRNG_data *rngbuf, long batch_size);
precise_t estimate_pi3_riemann(RedbearRNG_data *rngbuf, long batch_size);
precise_t rand_3sphere_norm(struct drand48_data *rngbuf);
void *threaded_calc_pi(void *arg);
void *threaded_calc_pi2(void *arg);

#endif //CALC_PI_MC_MAIN_H
