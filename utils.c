#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

void unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    _exit(0);
}

void sio_reverse(char s[]) {
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void sio_ltoa(long v, char s[], int b) {
    int c, i = 0;
    int neg = v < 0;

    if (neg)
        v = -v;

    do {
        s[i++] = ((c = (v % b)) < 10)  ?  c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);

    if (neg)
        s[i++] = '-';

    s[i] = '\0';
    sio_reverse(s);
}

size_t sio_strlen(char s[]) {
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}