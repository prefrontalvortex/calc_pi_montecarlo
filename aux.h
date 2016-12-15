//
// Created by mike on 13/12/16.
//

#ifndef CALC_PI_MC_AUX_H
#define CALC_PI_MC_AUX_H

#include <stdlib.h>
#include <stdio.h>
#include <quadmath.h>

#define CHECK(ccode,labl) checkpoint_stdprintf((ccode), "XP|%s\t%d\t%s\n", __FILE__, __LINE__, (labl))


typedef unsigned int uint128_t __attribute__((mode(TI))); // 16-BYTE MEGA LONG
typedef long double precise_t;
//typedef __float128 precise_t;

typedef struct _stopwatch {
    struct timespec startElapsed;
    struct timespec now;
    struct timespec endElapsed;
} stopwatch_t;

void checkpoint_stdprintf(int ccode, const char *format, ...);
void *emalloc(size_t numBytes);
FILE *e_fopen(const char *__filename, const char *__modes);
void die(const char *format, ...);
void startTimer(stopwatch_t *stopwatch);
double getElaspedTime(stopwatch_t *stopwatch);


#endif //CALC_PI_MC_AUX_H
