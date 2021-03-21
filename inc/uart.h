#ifndef UARTCONFIG_H
#define UARTCONFIG_H

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc16.h"

void closeUart();
float get_temperature(int);

#endif
