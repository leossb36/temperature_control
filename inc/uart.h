#ifndef UARTCONFIG_H
#define UARTCONFIG_H
#endif

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc16.h"

#define INTERNAL_TEMPERATURE 0XC1
#define REFERENCIAL_TEMPERATURE 0xC2

void closeUart();
float get_temperature(int);