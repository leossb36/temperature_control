#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "crc16.h"
#include "uart.h"
#include "i2clcd.h"
#include "pid.h"
#include "gpio.h"
#include "csv.h"
#include "bme280_sensor.h"
#include "control.h"
#include "menu.h"
#include <curses.h>

pthread_t tid;

void cancelProcess(int signal) {
    using_fan(0);
    using_resistor(0);
    ClrLcd();
    closeUart();
    close_bme();
    endwin();
    exit(0);
}

int main(int argc, char **argv) {

    signal(SIGINT, cancelProcess);

    pthread_create(&tid, NULL, &menu_execution, NULL);

    pthread_detach(tid);

    setupLCD();

    init_bme();
    pid_configura_constantes(5.0, 1.0, 5.0);

    while(1) {
        update_display();
        usleep(800000);
    }

    return 0;
}