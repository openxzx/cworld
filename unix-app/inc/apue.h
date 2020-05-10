/*
 * Our own header, to be included before all standard system headers.
 */

#ifndef _APUE_H
#define _APUE_H

#define _POSIX_C_SOURCE         200809L

#if defined(SOLARIS)    /* Solaris 10 */
#define _XOPEN_SOURCE   600
#else
#define _XOPEN_SOURCE   700
#endif

#include <sys/types.h>          /* Some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>        /* For winsize */

#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

#include <stdio.h>              /* For convenience */
#include <stdlib.h>             /* For convenience */
#include <stddef.h>             /* For offsetof */
#include <string.h>             /* For convenience */
#include <unistd.h>             /* For convenience */
#include <signal.h>             /* For SIG_ERR */

#define MAXLINE         4096    /* Max line length */

/*
 * Default file access permissions for new files.
 */
#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * Defau permissions for new directories.
 */
#define DIR_MODE       (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void Sigfunc(int);      /* For signal handlers */

#define min(a, b)       ((a) < (b) ? (a) : (b))
#define max(a, b)       ((a) > (b) ? (a) : (b))

/*
 * Defau permissions for new directories.
 */

#endif
