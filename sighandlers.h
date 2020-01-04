//
// Created by Ugnius on 2/22/2019.
//

#ifndef HW3_SIGHANDLERS_H
#define HW3_SIGHANDLERS_H

#include <signal.h>

extern volatile sig_atomic_t FLAG_PID;

// Reference used: http://www.cs.cmu.edu/afs/cs/academic/class/15213-f15/www/lectures/15-ecf-signals.pdf
// for SIGCHLD and sigsuspend

void init_handlers();

#endif //HW3_SIGHANDLERS_H
