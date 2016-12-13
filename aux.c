//
// Created by mike on 13/12/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aux.h"

void die(const char *format, ...) {
    // Print message and Kill process
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
//    fprintf(stderr, "%s x_x", msg);
    fflush(stderr);
    exit(EXIT_FAILURE);
}

void *emalloc(size_t numBytes){ /* Error checked malloc */
    void *buffer = malloc(numBytes);
    if(!buffer){
        fprintf(stderr, "Fatal error: malloc error. Out of memory. \n  x_X\n");
        exit(EXIT_FAILURE);
    }
    return buffer;
}