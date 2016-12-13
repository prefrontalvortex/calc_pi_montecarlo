//
// Created by mike on 13/12/16.
//

#ifndef CALC_PI_MC_KALMAN_H
#define CALC_PI_MC_KALMAN_H

#include "aux.h"

typedef struct _Kalman1D {
    precise_t obs;          // observations (normal about x, sigma=0.1);
    precise_t x_hat;        // a posteri estimate of x
    precise_t P;            // a posteri error estimate
    precise_t x_hat_minus;  // a priori estimate of x
    precise_t P_minus;      // a priori error estimate
    precise_t K_gain;       // gain or blending factor
    precise_t R_obsvar;     // estimate of measurement variance
    precise_t Q_procvar;    // process variance
    long iters;             // iterations run for
} Kalman1D;

Kalman1D *new_kalman1D(precise_t R_obsvar, precise_t Q_procvar, precise_t guess_x_hat, precise_t init_P);

precise_t kalman_observe(Kalman1D *kalman, precise_t obs);

#endif //CALC_PI_MC_KALMAN_H
