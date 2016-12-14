//
// Created by mike on 13/12/16.
//
#include <stdlib.h>
#include "kalman.h"
#include "aux.h"

Kalman1D *new_kalman1D(precise_t R_obsvar, precise_t Q_procvar, precise_t guess_x_hat, precise_t init_P) {
    Kalman1D *kalman = (Kalman1D*) emalloc(sizeof(Kalman1D));
    kalman->R_obsvar = R_obsvar;
    kalman->Q_procvar = Q_procvar;
    kalman->x_hat = guess_x_hat;
    kalman->P = init_P;
    kalman->K_gain = 0.1;
    kalman->iters = 0;
    return kalman;
}

SimpleAverage *new_SimpleAverage(long max_iters, int num_threads) {
    SimpleAverage *avglist = (SimpleAverage*) emalloc(sizeof(SimpleAverage));
    avglist->samples = (precise_t*) emalloc(max_iters * num_threads * sizeof(precise_t));
    avglist->iters = (long*) emalloc(num_threads * sizeof(long));
    avglist->idx = 0;
    avglist->total = 0;
    avglist->vartotal = 0;
    avglist->REAL_PI = 3.1415926535897932384626433832795028841971; // for diagnostic purposes

    return avglist;
}

precise_t kalman_observe(Kalman1D *kalman, precise_t obs) {
    // store priors
    kalman->x_hat_minus = kalman->x_hat;
    kalman->P_minus = kalman->P + kalman->Q_procvar;

// measurement update
    kalman->K_gain = kalman->P_minus/( kalman->P_minus + kalman->R_obsvar);
    kalman->x_hat = kalman->x_hat_minus + kalman->K_gain * (obs-kalman->x_hat_minus);
    kalman->P = (1 - kalman->K_gain) * kalman->P_minus;
    return kalman->x_hat;
}

precise_t simple_average_observe(SimpleAverage *avglist, precise_t obs) {
    // store priors
    avglist->samples[avglist->idx] = obs;
//    avglist->mean = simple_average_mean(avglist);
    avglist->obs = obs;
    avglist->total += obs;
    precise_t delta = (obs - avglist->REAL_PI);
    avglist->vartotal += (delta * delta);
    avglist->idx++;
    avglist->error = obs - avglist->mean;
    avglist->mean = avglist->total / (precise_t) avglist->idx;
    avglist->var = avglist->vartotal  / (precise_t) avglist->idx;
    return avglist->mean;
}

precise_t simple_average_mean(SimpleAverage *avglist) {
    long i;
    precise_t accu = 0;
    for (i= 0; i< avglist->idx; i++) {
        accu += avglist->samples[i];
    }
    return accu / (precise_t) avglist->idx;
}

