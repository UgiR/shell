#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "syswrappers.h"
#include "sighandlers.h"
#include "shell.h"

void initJob(job *j) {
    j->command = NULL;
    j->pgid = 0;
    j->first = NULL;
    j->inFile = NULL;
    j->outFile = NULL;
    j->append = 0;
    j->background = 0;
}

void delProcess(process *p) {
    int i;
    for (int i = 0; i < p->argc; ++i) {
        if (p->argv[i]) free(p->argv[i]);
    }
    free(p->argv);
}

void delJob(job *j) {
    if (j->command) free(j->command);
    if (j->inFile) free(j->inFile);
    if (j->outFile) free(j->outFile);

    process *p = j->first;
    process *tmp;
    while (p) {
        delProcess(p);
        tmp = p->next;
        free(p);
        p = tmp;
    }
}

void addToJob(job *j, process* p) {
    process *tmp = j->first;
    if (tmp == NULL) {
        j->first = p;
        return;
    }

    while (tmp->next != NULL)
        tmp = tmp->next;

    tmp->next = p;
}

int parseCommand(char *command, job *j) {
    char *token;
    command = strdup(command); // copy of command
    strtok(command, "\n"); // remove newline
    char *toFree = command;
    int i, argc_;
    size_t alloc;
    argc_ = 0;
    char **argv_;

    token = strtok(command, " ");
    while(token != NULL) {
        if (argc_ == 0) {
            argv_ = (char**)malloc(1 * sizeof(char*));
            alloc = 1;
        }
        else if (argc_ == alloc) { // resize
            argv_ = realloc(argv_, (alloc + 1) * sizeof(char*));
            alloc += 1;
        }

        if (strcmp(token, "|") == 0) {
            process *p = (process*)malloc(sizeof(process));
            argv_[argc_] = NULL;
            p->next = NULL;
            p->argv = argv_;
            p->argc = argc_;
            argc_ = 0;
            addToJob(j, p);
        }
        else if (strcmp(token, "&") == 0) {
            j->background = 1;
        }
        else if (strcmp(token, ">>") == 0) {
            j->append = 1;
            token = strtok(NULL, " ");
            j->outFile = strdup(token);
        }
        else if (strcmp(token, ">") == 0) {
            j->append = 0;
            token = strtok(NULL, " ");
            j->outFile = strdup(token);
        }
        else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            j->inFile = strdup(token);
        }
        else {
            argv_[argc_] = strdup(token);
            argc_++;

        }
        token = strtok(NULL, " ");
    }

    argv_ = realloc(argv_, (alloc + 1) * sizeof(char*));
    alloc += 1;
    process *p = (process*)malloc(sizeof(process));
    argv_[argc_] = NULL;
    p->next = NULL;
    p->argv = argv_;
    p->argc = argc_;
    argc_ = 0;
    addToJob(j, p);

    free(toFree);
}

int stringsEqual(char *s1, char *s2) {
    return strcmp(s1, s2) == 0;
}

int pwd() {
    // Get PATH_MAX limit for current directory
    long size = pathconf(".", _PC_PATH_MAX);

    char *path = (char*)malloc(size * sizeof(char));

    if (path != NULL) {
        path = getcwd(path, (size_t) size);
        printf("%s\n", path);
        free(path);
        return 0;
    }
    else { // memory allocation error
        return -1;
    }
}

int commandHandler(char *command) {
    job job;
    initJob(&job);
    parseCommand(command, &job);

    Signal(SIGTTIN, SIG_IGN);
    Signal(SIGTTOU, SIG_IGN);

    pid_t parent_pgid = getpid();
    pid_t child_pgid;
    Setpgid(parent_pgid, parent_pgid);

    int i = 0;
    int fdin, fdout, tmpin, tmpout;
    sigset_t mask, prev;
    tmpin = Dup(STDIN_FILENO);
    tmpout = Dup(STDOUT_FILENO);

    if (job.inFile != NULL) { // input file is defined
        fdin = Open(job.inFile, O_RDONLY);
    }
    else { // no input file defined
        fdin = Dup(tmpin);
    }
    Tcsetpgrp(tmpin, parent_pgid);
    process *currProcess = job.first;
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);
    Sigprocmask(SIG_BLOCK, &mask, &prev); /* Block SIGCHLD */
    while (currProcess) { // loop over all commands
        Dup2(fdin, STDIN_FILENO);
        Close(fdin);
        if (currProcess->next == NULL) { // last
            if (job.outFile != NULL) { // output redirect file defined
                if (job.append == 1)
                    fdout = open(job.outFile, O_CREAT|O_APPEND|O_WRONLY, 0666);
                else
                    fdout = open(job.outFile, O_CREAT|O_WRONLY, 0666);
            }
            else { // no output redirect file
                fdout = dup(tmpout);
            }
        }
        else {
            int fd[2];
            Pipe(fd);
            fdout = fd[1];
            fdin = fd[0];
        }
        Dup2(fdout, STDOUT_FILENO);
        Close(fdout);

        if (stringsEqual(currProcess->argv[0], "pwd")) {
            pwd();
        }
        else if (stringsEqual(currProcess->argv[0], "cd")) {
            chdir(currProcess->argv[1]);
        }
        else if (stringsEqual(currProcess->argv[0], "exit")) {
            delJob(&job);
            exit(0);
        }
        else {
            int p_ = Fork();
            if (p_ == 0) {
                p_ = getpid();
                Setpgid(p_, p_);
                if (!job.background) {
                    Tcsetpgrp(tmpin, p_);
                }
                Execvp(currProcess->argv[0], currProcess->argv);
                _exit(1);
            }
            else {
                Setpgid(p_, p_);
                FLAG_PID = 0;
                if (!job.background) {
                    while (!FLAG_PID)
                        Sigsuspend(&prev);
                }
            }
        }

        currProcess = currProcess->next;
    }
    Sigprocmask(SIG_SETMASK, &prev, NULL);

    Dup2(tmpin, STDIN_FILENO);
    Dup2(tmpout, STDOUT_FILENO);
    Close(tmpin);
    Close(tmpout);
    Tcsetpgrp(STDIN_FILENO, parent_pgid);

    delJob(&job);
    return true;
}

void mainInputLoop() {
    char input[129];
    while (true) {
        printf("user >");
        fgets(input, 129, stdin);
        commandHandler(input);
    }
}

int main() {
    init_handlers();

    mainInputLoop();

    return 0;
}
