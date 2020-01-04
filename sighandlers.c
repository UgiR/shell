#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "syswrappers.h"
#include "utils.h"
#include "sighandlers.h"

static struct sigaction default_sa;
static struct sigaction sigint_sa;
static struct sigaction sigchld_sa;
static struct sigaction sigterm_sa;
static struct sigaction sigtstp_sa;
static struct sigaction sigcont_sa;

static void sigint_handler(int sig) {
    Write(STDOUT_FILENO, "\nCaught: Interrupt\n", 18);
    Sigaction(SIGINT, &default_sa, NULL);
    raise(SIGINT);
}

static void sigchld_handler(int sig) {
    int status;
    char status_str[32];
    char pid_str[32];
    sigset_t mask_all, prev_all;
    int errno_ = errno;

    sigfillset(&mask_all); // TODO: make wrapper
    pid_t pid = Waitpid(-1, &status, 0);

    if (WIFEXITED(status)) {
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        sio_ltoa(pid, pid_str, 10);
        sio_ltoa(WEXITSTATUS(status), status_str, 10);
        Write(STDOUT_FILENO, "pid:", 4);
        Write(STDOUT_FILENO, pid_str, sio_strlen(pid_str));
        Write(STDOUT_FILENO, " status:", 8);
        Write(STDOUT_FILENO, status_str, sio_strlen(status_str));
        Write(STDOUT_FILENO, "\n", 1);
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    else {
        Write(STDOUT_FILENO, "Abnormal exit\n", 14);
    }

    FLAG_PID = pid;
    errno = errno_;
}

static void sigterm_handler(int sig) {
    Kill(0, SIGTERM);
    Write(STDOUT_FILENO, "Caught: Terminated\n", 19);
    exit(0);
}

static void sigtstp_handler(int sig) {

}

static void sigcont_handler(int sig) {
    int errno_ = errno;
    Write(STDOUT_FILENO, "CS361 >", 7);
    errno = errno_;

}

void init_handlers() {
    // Set handler functions to structs
    default_sa.sa_handler = SIG_DFL; // defines default signal
    sigint_sa.sa_handler = sigint_handler;
    sigchld_sa.sa_handler = sigchld_handler;
    sigterm_sa.sa_handler = sigterm_handler;
    sigtstp_sa.sa_handler = sigtstp_handler;
    sigcont_sa.sa_handler = sigcont_handler;

    // Set signal blocks
    sigemptyset(&default_sa.sa_mask);
    sigemptyset(&sigint_sa.sa_mask);
    sigemptyset(&sigchld_sa.sa_mask);
    sigemptyset(&sigterm_sa.sa_mask);
    sigemptyset(&sigtstp_sa.sa_mask);
    sigemptyset(&sigcont_sa.sa_mask);

    // Set flags
    sigint_sa.sa_flags = SA_RESTART;
    sigchld_sa.sa_flags = SA_RESTART;
    sigterm_sa.sa_flags = SA_RESTART;
    sigtstp_sa.sa_flags = SA_RESTART;
    sigcont_sa.sa_flags = SA_RESTART;

    // Set handlers to sigs
    Sigaction(SIGINT, &sigint_sa, NULL);
    Sigaction(SIGCHLD, &sigchld_sa, NULL);
    Sigaction(SIGTERM, &sigterm_sa, NULL);
    //Sigaction(SIGTSTP, &sigtstp_sa, NULL); // Preserve default
    Sigaction(SIGCONT, &sigcont_sa, NULL);
}