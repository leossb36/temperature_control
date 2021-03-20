#include <curses.h>
#include <string.h>
#include <unistd.h>

void menu() {
    int xMax, yMax, new_x, new_y;
    int commands_box_size = 10;
    int key = ERR;

    int hightlight = 0;

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    char options[2][30] = {"[1] - Temperatura Automática", "[2] - Temperatura Manual"};

    // set up initial windows
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *data_box = newwin(yMax - commands_box_size, xMax, 0, 0);
    WINDOW *command_box = newwin(commands_box_size, xMax, yMax - commands_box_size, 0);

    box(data_box, 0, 0);
    box(command_box, 0, 0);

    while(1) {

        for(int i = 0; i < 2; i++) {
            if (i == hightlight) {
                wattron(command_box, A_REVERSE);
            }
            mvwprintw(command_box, 2, i+i*30 + 2, options[i]);
            wattroff(command_box, A_REVERSE);
        }
        switch(key) {
            case KEY_LEFT:
                hightlight = 0;
                break;
            case KEY_RIGHT:
                hightlight = 1;
                break;
            default:
                break;
        }

        getmaxyx(stdscr, new_y, new_x);

        if (new_y != yMax || new_x != xMax) {
            xMax = new_x;
            yMax = new_y;

            wresize(data_box, new_y - commands_box_size, new_x);
            wresize(command_box, commands_box_size, new_x);
            mvwin(command_box, new_y - commands_box_size, 0);

            wclear(stdscr);
            wclear(data_box);
            wclear(command_box);

            box(data_box, 0, 0);
            box(command_box, 0, 0);
        }
        // draw to our windows
        mvwprintw(data_box, 0, 1, "Dados");
        mvwprintw(command_box, 0, 1, "Comandos");

        // refresh each window
        wrefresh(stdscr);
        wrefresh(data_box);
        wrefresh(command_box);

        key = getch();
        usleep(10000);
    }
    endwin();
}