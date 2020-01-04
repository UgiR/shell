//
// Created by Ugnius on 2/21/2019.
//

#ifndef HW3_SHELL_H
#define HW3_SHELL_H

#include <stddef.h>
#include <signal.h>
#include <sys/types.h>

#define ARG_MAX sysconf(_SC_ARG_MAX)
#define true 1
#define false 0

extern volatile sig_atomic_t FLAG_PID;

/*
 * Reference used: https://www.gnu.org/software/libc/manual/html_node/Data-Structures.html#Data-Structures
 */
typedef struct process {
    struct process *next;
    pid_t pid;
    char **argv;
    int argc;
} process;

/*
 * Reference used: https://www.gnu.org/software/libc/manual/html_node/Data-Structures.html#Data-Structures
 */
typedef struct job {
    char *command;
    pid_t pgid;
    process *first;
    char *inFile;
    char *outFile;
    int append;
    int background;
} job;

int parseCommand(char *command, job *j);

int stringsEqual(char *s1, char *s2);

// Pipe/redirects
// Reference used: https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf
int commandHandler(char *command);

void mainInputLoop();

#endif //HW3_SHELL_H
