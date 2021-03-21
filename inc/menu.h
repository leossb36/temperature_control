#ifndef MENU_H
#define MENU_H

#include <curses.h>

extern volatile int manual;

void *menu_execution();
void write_on_data_box(WINDOW *, WINDOW *, WINDOW *, int , int , int, int);

#endif