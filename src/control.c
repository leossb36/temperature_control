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

volatile float temp_int, temp_ext, temp_pot, temp_ref, pid, resistor, fan;
volatile float referencial_temp = 35;

void update_display() {

    temp_int = get_temperature(INTERNAL_TEMPERATURE);
    temp_pot = get_temperature(POT_TEMPERATURE);

    temp_ext = get_temperature_bme();

    temp_ref = getReferencialTemp();

    if (temp_int > 0 && temp_ext > 0 && temp_ref > 0) {
        pid_atualiza_referencia(temp_ref);
        pid = pid_controle(temp_int);

        if (pid > 0) {
            using_resistor(pid);
            using_fan(0);
            resistor = pid;
            fan = 0;
        }
        else {
            if(pid <= -40) {
                using_fan(-1 * pid);
                using_resistor(0);
                fan = -1*pid;
                resistor = 0;
            }
            else if (pid <= 0 && pid >= -40) {
                using_resistor(0);
                using_fan(0);
                fan = 0;
                resistor = 0;
            }
        }

        writeData();
    }
    usleep(1000000);
}

void writeData() {
    char line_1[20], line_2[20];
    sprintf(line_1, "TR:%.1f", temp_ref);
    sprintf(line_2, "TI:%.1f TE:%.1f", temp_int, temp_ext);
    writeOnLCD(line_1, line_2);
    writeOnCSVFile(temp_int, temp_ref, temp_ext, resistor, fan);
}

float getReferencialTemp() {
    if (manual == 1)
        return referencial_temp;
    return temp_pot;
}