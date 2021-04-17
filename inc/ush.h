#ifndef USH_H
#define USH_H

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <termios.h>

#include "functions.h"
#include "macros.h"
#include "libmx.h"
#include "struct.h"

extern struct termios stored_settings;

#endif /* USH */
