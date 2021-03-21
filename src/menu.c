#include "menu.h"
#include <curses.h>
#include "control.h"

volatile int manual = 0;

void *menu_execution() {

    int xMax, yMax, new_x, new_y;
    int commands_box_size = 5;
    int key = ERR;

    int hightlight = 0;

    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    char options[2][30] = {"[1] - POTENCIOMETRO", "[2] - TEMPERATURA MANUAL"};

    // set up initial windows
    getmaxyx(stdscr, yMax, xMax);

    int databox_size[4] = { yMax - commands_box_size, xMax/4,  0, 0 };
    int commandbox_size[4] = { commands_box_size, xMax/2 + 1, yMax - commands_box_size, 0 };
    int resultbox_size[4] = { yMax - commands_box_size, xMax/6, 0, xMax/4 };
    int referencialbox_size[4] = { yMax - commands_box_size, xMax/10, 0, xMax/2 - xMax/12 - 2 };

    WINDOW *data_box = newwin(databox_size[0], databox_size[1], databox_size[2], databox_size[3]);
    WINDOW *command_box = newwin(commandbox_size[0], commandbox_size[1], commandbox_size[2], commandbox_size[3]);
    WINDOW *result_box = newwin(resultbox_size[0], resultbox_size[1], resultbox_size[2], resultbox_size[3]);
    WINDOW *referencial_box = newwin(referencialbox_size[0], referencialbox_size[1], referencialbox_size[2], referencialbox_size[3]);

    box(data_box, 0, 0);
    box(command_box, 0, 0);
    box(result_box, 0, 0);
    box(referencial_box, 0, 0);

    while(1) {
        for(int i = 0; i < 2; i++) {
            if (i == hightlight) {
                wattron(command_box, A_REVERSE);
            }
            mvwprintw(command_box, 2, i+i*30 + 8, options[i]);
            wattroff(command_box, A_REVERSE);
        }

        if (new_y != yMax || new_x != xMax) {
            xMax = new_x;
            yMax = new_y;
            getmaxyx(stdscr, new_y, new_x);

            wresize(data_box, new_y - commands_box_size, new_x/4);
            wresize(command_box, commands_box_size, new_x/2 + 1);
            wresize(result_box, new_y - commands_box_size, new_x/6);
            wresize(referencial_box, new_y - commands_box_size, new_x/10);
            mvwin(command_box, new_y - commands_box_size, 0);

            wclear(stdscr);
            wclear(data_box);
            wclear(command_box);
            wclear(result_box);
            wclear(referencial_box);

            box(data_box, 0, 0);
            box(command_box, 0, 0);
            box(result_box, 0, 0);
            box(referencial_box, 0, 0);
        }

        key = getch();

        switch(key) {
            case KEY_LEFT:
                hightlight = 0;
                manual = 0;
                break;
            case KEY_RIGHT:
                hightlight = 1;
                manual = 1;
                break;
            case KEY_UP:
                if(manual) {
                    referencial_temp++;
                    if(referencial_temp > 99) {
                        referencial_temp = 99;
                    }
                }
                break;
            case KEY_DOWN:
                if(manual) {
                    referencial_temp--;
                    if(referencial_temp < (int)temp_ext) {
                        referencial_temp = (int)temp_ext;
                    }
                }
                break;
            default:
                break;
        }

        // draw to our windows
        mvwprintw(data_box, 0, databox_size[1]/2, "Dados");
        mvwprintw(command_box, 0, commandbox_size[1]/4 + commandbox_size[1]/5, "Comandos");
        mvwprintw(result_box, 0, resultbox_size[1]/3, "Resultados");
        mvwprintw(referencial_box, 0, referencialbox_size[1]/3, "Ajuste");

        write_on_data_box(data_box, result_box, referencial_box, databox_size[1], resultbox_size[1]/3 + 3, referencialbox_size[1]/3 + 3, referencialbox_size[0] - 2);

        wrefresh(stdscr);
        wrefresh(data_box);
        wrefresh(command_box);
        wrefresh(result_box);
        wrefresh(referencial_box);
    }
}

void write_on_data_box(WINDOW *data_box, WINDOW *result_box, WINDOW *adjust_box, int dataSize, int resultSize, int adjustSize, int referencialSize) {
    mvwprintw(data_box, 2, dataSize/4, "TEMPERATURA INTERNA");
    mvwprintw(data_box, 3, dataSize/4, "TEMPERATURA REFERENTE");
    mvwprintw(data_box, 4, dataSize/4, "TEMPERATURA EXTERNA");
    mvwprintw(data_box, 5, dataSize/4, "TEMPERATURA POTENCIOMETRO");
    mvwprintw(data_box, 6, dataSize/4, "RESISTOR");
    mvwprintw(data_box, 7, dataSize/4, "FAN");

    mvwprintw(result_box, 2, resultSize, "%.2f ºC", temp_int);
    mvwprintw(result_box, 3, resultSize, "%.2f ºC", temp_ref);
    mvwprintw(result_box, 4, resultSize, "%.2f ºC", temp_ext);
    mvwprintw(result_box, 5, resultSize, "%.2f ºC", temp_pot);
    mvwprintw(result_box, 6, resultSize, "%.2f", resistor);
    mvwprintw(result_box, 7, resultSize, "%.2f", fan);

    mvwprintw(adjust_box, 4, adjustSize, "PGUP");
    mvwprintw(adjust_box, 7, adjustSize, "PGDOWN");
    mvwprintw(adjust_box, referencialSize - 2, adjustSize, "%d ºC", (int) referencial_temp);
}