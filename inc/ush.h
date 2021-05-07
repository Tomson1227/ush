#ifndef USH_H
#define USH_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <spawn.h>
#include <fcntl.h>

#include "functions.h"
#include "macros.h"
#include "libmx.h"
#include "struct.h"
#include "keydef.h"
#include "process.h"

extern char **environ;
extern struct termios stored_settings;

#endif /* USH */
