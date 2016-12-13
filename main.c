//
// Created by mike on 12/12/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
//#include <random>
#include <quadmath.h>
#include <time.h>
#include "main.h"
#include "aux.h"
#include "argparse.h"
#include "rng.h"
#include "kalman.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"



precise_t eps_to_alpha(precise_t epsilon);
precise_t estimate_pi3(struct drand48_data *rngbuf, long batch_size);
precise_t rand_3sphere_norm(struct drand48_data *rngbuf);
void *threaded_calc_pi(void *arg);

int NUM_THREADS;
pthread_mutex_t lock_x;
precise_t REAL_PI;

typedef struct _thread_data_t {
    int tid;
    long batch_size;
    Kalman1D *kalman;
    precise_t pi_calc;
    precise_t pi_est;
    char *filename;
    struct drand48_data *rngbuf;

} thread_data_t;


int main(int argc, char **argv) {
    int rc, NDIM = 3;
    int seed, randVecDims, i, n, d, th;
    long iter=0, lseed, batch_size;
    precise_t pi_est, r_norm, rad_calc[8];
    precise_t pi_calc, ratio, prior, epsilon=99, delta=99, alpha=0.1;
    REAL_PI = 3.1415926535897932384626433832795028841971; //3.141592653589;
    double d_batch_size;
    double R_init = 1e-3, Q_init = 1e-6;

    uint128_t count_in = 0, count_all =0;

    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    __time_t tmp_seconds = now.tv_sec;

    //// Grab parameters from the command line arguments
    arg_t *args = NULL;
    parser_populate(&args, argc, argv);
    parse_assign_i(&NUM_THREADS, "-t", args, "1");
    parse_assign_d(&d_batch_size, "-b", args, "1e-7");
    batch_size = (long) d_batch_size;


    //// Set up filename for output
    char datafilename[256];
    sprintf(datafilename, "out/%d_%ld_pi_kalman.csv", NUM_THREADS, (long) tmp_seconds);
    FILE *pifile = fopen(datafilename, "w");
    fprintf(pifile, "thread,iters,K_gain,pi_batch,pi_kalman,error\n");
    fclose(pifile);



    fprintf(stderr, "Size of uint128: %d-bit\n Precise_t: %d-bit\n", 8*sizeof(uint128_t), 8*sizeof(precise_t));
    fprintf(stderr, " Num Iters/Batch: %ld\n Num Threads: %d\n", batch_size, NUM_THREADS);

    FILE * randfile = fopen("/dev/random", "r");

    if (randfile == NULL)   die("Fatal error! Can't open file!\n");
    lseed = rand_long_from_file(randfile);
    fclose(randfile);
    seed = (int) lseed;
    srand(seed);
//    for (i = 0; i< 100; i++) {
//        printf("%lf\n", rand_uniform32(rand()));
//    }

    pi_est = 3.14;//estimate_pi3(seed, (int) batch_size);



    Kalman1D *kalman = new_kalman1D(R_init, Q_init, pi_est, 1.0);
    thread_data_t payload[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    struct drand48_data rngbuf[NUM_THREADS];
    if (pthread_mutex_init(&lock_x, NULL) != 0) {
        fprintf(stderr, "\nFatal: Mutex init failed\n"), exit(EXIT_FAILURE);
    }


    for (th=0; th<NUM_THREADS; th++) {
        srand48_r(lseed + th, &(rngbuf[th]));
        payload[th].tid = th;
        payload[th].kalman = kalman;
        payload[th].batch_size = batch_size;
        payload[th].rngbuf = &(rngbuf[th]);
        payload[th].filename = datafilename;
    }

    for (th=0; th<NUM_THREADS; th++) {
        rc = pthread_create(&threads[th], NULL, threaded_calc_pi, (void *) &payload[th]);

    }
    for (th = 0; th < NUM_THREADS; th++){
        printf("\n<M>Calling join on thread: %d\n", th);
        pthread_join(threads[th], NULL);
        printf("<M>... %d joined. \n", th);
    }

    return EXIT_SUCCESS;
}

precise_t estimate_pi3(struct drand48_data *rngbuf, long batch_size) {
    precise_t vec;
    uint128_t count_in = 0, count_all =0;
    long iter;
    for (iter=0; iter<batch_size; iter++) {
        vec = rand_3sphere_norm(rngbuf);
        if (vec < 1.0) count_in++;
        count_all++;
    }
    precise_t ratio = (precise_t) count_in / (precise_t) count_all;
//    printf("Ratio: %lf\n", ratio);
    // v = 4/3 pi r^3 ... pi/6
    return ratio*6;
}

precise_t rand_3sphere_norm(struct drand48_data *rngbuf) {
    int d;
    precise_t r_comp, r_norm = 0;
    for (d=0; d<3   ; d++) {
        r_comp = rand_uniform_r(rngbuf); // on 0-1
        r_norm += r_comp * r_comp;
    }
    return sqrt( (double) r_norm);
//    if (r_norm < 1) return 1;
//    return 0;

}


precise_t crit_kalman_observe(Kalman1D *kalman, precise_t obs) {
    // Threadsafe Kalman update
}

void *threaded_calc_pi(void *arg) {
    FILE *pifile;
    int i, tid;
    thread_data_t *payload = (thread_data_t *) arg;
    tid = payload->tid;
    long iter = 0, batch_size = payload->batch_size;
    Kalman1D *kalman = payload->kalman;
    precise_t pi_calc, pi_est;

    printf("\n<%d> Hi! start.\n", tid);

    while (1) {
        pi_calc = estimate_pi3(payload->rngbuf, batch_size);
        // ========== ENTER CRITICAL REGION *******
        pthread_mutex_lock(&lock_x);
//        fprintf(stdout, "%d locked\n", tid);
        pi_est = kalman_observe(kalman, pi_calc);
        pifile = fopen(payload->filename, "a");
        fprintf(pifile, "%d,%ld,%le,%.16lf,%.16lf,%.16lf\n", tid, batch_size*iter, (double) kalman->K_gain,
                (double) pi_calc, (double) pi_est, (double) pi_est - 3.141592653589);
        fclose(pifile);
        pthread_mutex_unlock(&lock_x);
        // ========== LEAVE CRITICAL REGION ***
        if (tid == 0) {
            fprintf(stdout, "<%d>Iter: %5ld Kalman K: %le \n", tid, iter, (double) kalman->K_gain);
            fprintf(stdout, "<%d>Kalman P:  %.12lf\n", tid, (double) kalman->P);
            fprintf(stdout, "<%d>Pi Calc :  %.12lf\n", tid, (double) pi_calc);
            fprintf(stdout, "<%d>Pi IIR  :  %.12lf\n", tid, (double) pi_est);
            fprintf(stdout, "<%d>Diff    : %+.12lf\n", tid, (double) (pi_calc - REAL_PI));
            fprintf(stdout, "Diff IIR: %+.12lf\n", (double) (pi_est - REAL_PI));

        }

        iter++;

    }

    pthread_exit(NULL);
}


#pragma clang diagnostic pop