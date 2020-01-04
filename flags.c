/*
 * Global flags
 */

#include <signal.h>

/*
 * Flag set by SIGCHLD handler corresponding to PID of reaped process
 */
volatile sig_atomic_t FLAG_PID = 0;
