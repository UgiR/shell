#ifndef HW3_UTILS_H
#define HW3_UTILS_H

#include <stddef.h>

/* Print unix error
 * Code attribution: CSAPP3e p738
 */
void unix_error(char *msg);

/* sio_reverse - Reverse a string (from K&R)
 * Code attribution: http://csapp.cs.cmu.edu/3e/ics3/code/src/csapp.c
 */
void sio_reverse(char s[]);

/* sio_ltoa - Convert long to base b string (from K&R)
 * Code attribution: http://csapp.cs.cmu.edu/3e/ics3/code/src/csapp.c
 */
void sio_ltoa(long v, char s[], int b);

/* sio_strlen - Return length of string (from K&R)
 * Code attribution: http://csapp.cs.cmu.edu/3e/ics3/code/src/csapp.c
 */
size_t sio_strlen(char s[]);

#endif //HW3_UTILS_H
