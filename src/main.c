#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "crc16.h"
#include "uart.h"
#include "i2clcd.h"
#include "pid.h"
#include "gpio.h"
#include "csv.h"
#include "bme280_sensor.h"


void cancelProcessUart(int signal) {
    using_fan(0);
    using_resistor(0);
    closeUart();
    close_bme();
    exit(0);
}

int main(void) {

    signal(SIGINT, cancelProcessUart);

    float ti, tr, te;
    double kp, ki, kd;
    kp = 5.0;
    ki = 1.0;
    kd = 5.0;
    float pid = 0.0;
    float resistor, fan;

    char line_1[20], line_2[20];

    setupLCD();
    init_bme();
    pid_configura_constantes(kp, ki, kd);

    while(1) {
        ti = get_temperature(INTERNAL_TEMPERATURE);
        tr = get_temperature(REFERENCIAL_TEMPERATURE);
        te = get_temperature_bme();

        pid_atualiza_referencia(tr);
        pid = pid_controle(ti);

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
            else if (pid <= 0 && pid >= -40){
                using_resistor(0);
                using_fan(0);
                fan = 0;
                resistor = 0;
            }
        }

        sprintf(line_1, "TI:%.2f TR:%.2f", ti, tr);
        sprintf(line_2, "TE:%.2f", te);
        writeOnLCD(line_1, line_2);
        writeOnCSVFile(ti, tr, te, resistor, fan);
        usleep(1000000);
    }

    return 0;
}
