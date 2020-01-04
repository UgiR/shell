#ifndef HW3_SYSWRAPPERS_H
#define HW3_SYSWRAPPERS_H

#include <sys/types.h>
#include <signal.h>

// Wrappers based on same concept used in Fork() (as described in CSAPP book)
// Function references used to write wrappers:
//  * http://pubs.opengroup.org/onlinepubs/009696899/functions/

typedef void (*sighandler_t)(int);

// Error-handling wrapper for fork()
// Code Attribution: CSAPP3e p738
pid_t Fork(void);

int Kill(pid_t pid, int sig);

int Pause(void);

ssize_t Write(int fildes, const void *buf, size_t nbyte);

pid_t Wait(int *status);

pid_t Waitpid(pid_t pid, int *status, int options);

int Sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict aact);

int Pipe(int fildes[2]);

int Dup(int fildes);

int Dup2(int fildes, int fildes2);

int Open(const char *pathname, int flags);

int Close(int fildes);

int Execvp(const char *file, char *const argv[]);

int Sigprocmask(int how, const sigset_t *set, sigset_t *oset);

int Sigsuspend(const sigset_t *mask);

int Sigemptyset(sigset_t *set);

int Sigaddset(sigset_t *set, int signo);

int Sigfillset(sigset_t *set);

int Setpgid(pid_t pid, pid_t pgid);

int Tcsetpgrp(int fildes, pid_t pgid_id);

sighandler_t Signal(int signum, sighandler_t handler);

#endif //HW3_SYSWRAPPERS_H
