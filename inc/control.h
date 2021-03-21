#ifndef CONTROL_H
#define CONTROL_H

#define INTERNAL_TEMPERATURE 0XC1
#define POT_TEMPERATURE 0xC2

extern volatile float temp_int, temp_ext, temp_pot, temp_ref, pid, resistor, fan;
extern volatile float referencial_temp;

void update_display();
void writeData();
float getReferencialTemp();

#endif