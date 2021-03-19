#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "crc16.h"
#include "uart.h"
#include "i2clcd.h"
#include "bme280_sensor.h"


void cancelProcessUart(int signal) {
    closeUart();
    close_bme();
    exit(0);
}

int main(void) {

    signal(SIGINT, cancelProcessUart);

    float ti, tr, te;

    char line_1[20], line_2[20];

    setupLCD();
    init_bme();

    while(1) {
        ti = get_temperature(INTERNAL_TEMPERATURE);
        tr = get_temperature(REFERENCIAL_TEMPERATURE);
        te = get_temperature_bme();

        sprintf(line_1, "TI:%.2f TR:%.2f", ti, tr);
        sprintf(line_2, "TE:%.2f ::", te);

        writeOnLCD(line_1, line_2);

        sleep(1);
    }
    return 0;
}
