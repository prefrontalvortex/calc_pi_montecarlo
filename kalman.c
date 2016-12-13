//
// Created by mike on 13/12/16.
//
#include <stdlib.h>
#include "kalman.h"
#include "aux.h"

Kalman1D *new_kalman1D(precise_t R_obsvar, precise_t Q_procvar, precise_t guess_x_hat, precise_t init_P) {
    Kalman1D *kalman = (Kalman1D*) malloc(sizeof(Kalman1D));
    kalman->R_obsvar = R_obsvar;
    kalman->Q_procvar = Q_procvar;
    kalman->x_hat = guess_x_hat;
    kalman->P = init_P;
    kalman->K_gain = 0.1;
    kalman->iters = 0;
    return kalman;
}

/*
z = np.random.normal(x,0.1,size=sz) // observations (normal about x, sigma=0.1);

precise_t Q = 1e-5; // process variance

// allocate space for arrays
xhat=np.zeros(sz);      // a posteri estimate of x
P=np.zeros(sz);         // a posteri error estimate
xhatminus=np.zeros(sz); // a priori estimate of x
Pminus=np.zeros(sz);    // a priori error estimate
K=np.zeros(sz);         // gain or blending factor

R = 0.1**2; // estimate of measurement variance, change to see effect
*/
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