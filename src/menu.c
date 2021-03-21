#include "menu.h"
#include <curses.h>
#include "control.h"

volatile int manual = 0;

void *menu_execution() {

    int box_size_x, box_size_y;
    int commands_box_size = 3;
    int key = ERR;

    box_size_x = box_size_y = 30;

    int hightlight = 0;

    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    char options[2][30] = {"[1] - POTENCIOMETRO", "[2] - TEMPERATURA MANUAL"};

    // set up initial windows
    getmaxyx(stdscr, box_size_y, box_size_x);

    WINDOW *data_box = newwin(box_size_y/4 - commands_box_size - 1, box_size_x/4,  0, 0 );
    WINDOW *command_box = newwin(commands_box_size, box_size_x/2 + 2, box_size_y/5 - 1, 0);
    WINDOW *result_box = newwin(box_size_y/4 - commands_box_size - 1, box_size_x/6, 0, box_size_x/4);
    WINDOW *referencial_box = newwin(box_size_y/4 - commands_box_size - 1, box_size_x/10, 0, box_size_x/2 - box_size_x/12 - 2);

    box(data_box, 0, 0);
    box(command_box, 0, 0);
    box(result_box, 0, 0);
    box(referencial_box, 0, 0);

    while(1) {
        for(int i = 0; i < 2; i++) {
            if (i == hightlight) {
                wattron(command_box, A_REVERSE);
            }
            mvwprintw(command_box, 1, i+i*50 + box_size_x/10, options[i]);
            wattroff(command_box, A_REVERSE);
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
        mvwprintw(data_box, 0, (box_size_x/4)/2, "Dados");
        mvwprintw(command_box, 0, (box_size_x/2 + 2)/4 + (box_size_x/2 + 2)/5, "Comandos");
        mvwprintw(result_box, 0, (box_size_x/6)/3, "Resultados");
        mvwprintw(referencial_box, 0, (box_size_x/10)/3 + 2, "Ajuste");

        write_on_data_box(data_box, result_box, referencial_box, (box_size_x/4), (box_size_x/6)/3 + 3, (box_size_x/10)/3 + 3, box_size_y/4);

        wrefresh(stdscr);
        wrefresh(data_box);
        wrefresh(command_box);
        wrefresh(result_box);
        wrefresh(referencial_box);
    }
}

void write_on_data_box(WINDOW *data_box, WINDOW *result_box, WINDOW *adjust_box, int dataSize, int resultSize, int adjustSize, int referencialSize) {
    mvwprintw(data_box, 1, dataSize/4, "TEMPERATURA INTERNA");
    mvwprintw(data_box, 2, dataSize/4, "TEMPERATURA REFERENTE");
    mvwprintw(data_box, 3, dataSize/4, "TEMPERATURA EXTERNA");
    mvwprintw(data_box, 4, dataSize/4, "TEMPERATURA POTENCIOMETRO");
    mvwprintw(data_box, 5, dataSize/4, "PID");
    mvwprintw(data_box, 6, dataSize/4, "RESISTOR");
    mvwprintw(data_box, 7, dataSize/4, "FAN");

    mvwprintw(result_box, 1, resultSize, "%.2f ºC", temp_int);
    mvwprintw(result_box, 2, resultSize, "%.2f ºC", temp_ref);
    mvwprintw(result_box, 3, resultSize, "%.2f ºC", temp_ext);
    mvwprintw(result_box, 4, resultSize, "%.2f ºC", temp_pot);
    mvwprintw(result_box, 5, resultSize, "%.2f", pid);
    mvwprintw(result_box, 6, resultSize, "%d", (int)resistor);
    mvwprintw(result_box, 7, resultSize, "%d", (int)fan);

    mvwprintw(adjust_box, 4, adjustSize - 3, "PGUP");
    mvwprintw(adjust_box, 4, adjustSize + 3, "PGDOWN");
    mvwprintw(adjust_box, referencialSize - 8, adjustSize, "%d ºC", (int) referencial_temp);
}