#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#include "utils.h"
#include "syswrappers.h"

pid_t Fork(void) {
    pid_t pid;
    if ((pid = fork()) < 0)
        unix_error("fork error");
    return pid;
}

int Kill(pid_t pid, int sig) {
    int ret;
    if (ret = kill(pid, sig) == -1)
        unix_error("kill error");
    return ret;
}

int Pause(void) {
    return pause();
}

ssize_t Write(int fildes, const void *buf, size_t nbyte) {
    ssize_t ret;
    if (ret = write(fildes, buf, nbyte) == -1)
        unix_error("write error");
    return ret;
}

pid_t Wait(int *status) {
    pid_t ret;
    if ((ret = wait(status) == -1))
        unix_error("wait error");
    return ret;
}

pid_t Waitpid(pid_t pid, int *status, int options) {
    pid_t ret;
    if ((ret = waitpid(pid, status, options)) == -1)
        unix_error("waitpid error");
    return ret;
}

int Sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict aact) {
    int ret;
    if ((ret = sigaction(sig, act, aact)) == -1)
        unix_error("sigaction error");
    return ret;
}

int Pipe(int fildes[2]) {
    int ret;
    if ((ret = pipe(fildes)) == -1) {
        unix_error("pipe error");
    }
    return ret;
}

int Dup(int fildes) {
    int ret;
    if ((ret = dup(fildes)) == -1)
        unix_error("dup error");
    return ret;
}

int Dup2(int fildes, int fildes2) {
    int ret;
    if ((ret = dup2(fildes, fildes2)) == -1)
        unix_error("dup2 error");
    return ret;
}

int Open(const char *pathname, int flags) {
    int ret;
    if ((ret = open(pathname, flags)) == -1)
        unix_error("open error");
    return ret;
}
/*
int Open(const char *pathname, int flags, mode_t mode) {
    int ret;
    if ((ret = opnen(pathname, flags, mode)) == -1)
        unix_error("open error");
    return ret;
}
*/
int Close(int fildes) {
    int ret;
    if ((ret = close(fildes)) == -1)
        unix_error("close error");
    return ret;
}

int Execvp(const char *file, char *const argv[]) {
    int ret;
    if ((ret = execvp(file, argv)) == -1)
        unix_error("execvp error");
    return ret;
}

int Sigprocmask(int how, const sigset_t *set, sigset_t *oset) {
    int ret;
    if ((ret = sigprocmask(how, set, oset)) == -1)
        unix_error("sigprocmask error");
    return ret;
}

int Sigsuspend(const sigset_t *mask) {
    int ret;
    if ((ret = sigsuspend(mask)) == -1)
        //unix_error("sigsuspend error");
    return ret;
}

int Sigemptyset(sigset_t *set) {
    int ret;
    if ((ret = sigemptyset(set)) == -1)
        unix_error("sigemptyset error");
    return ret;
}

int Sigaddset(sigset_t *set, int signo) {
    int ret;
    if ((ret = sigaddset(set, signo)) == -1)
        unix_error("sigaddset error");
    return ret;
}

int Sigfillset(sigset_t *set) {
    int ret;
    if ((ret = sigfillset(set)) == -1)
        unix_error("sigfillset error");
    return ret;
}

int Setpgid(pid_t pid, pid_t pgid) {
    int ret;
    if ((ret = setpgid(pid, pgid)) == -2)
        unix_error("setpgid error");
    return ret;
}

int Tcsetpgrp(int fildes, pid_t pgid_id) {
    int ret;
    if ((ret = tcsetpgrp(fildes, pgid_id)) == -1)
        unix_error("tcsetpgrp error");
    return ret;
}

sighandler_t Signal(int signum, sighandler_t handler) {
    sighandler_t ret;
    if ((ret = signal(signum, handler)) == SIG_ERR)
        unix_error("signal error");
    return ret;
}
