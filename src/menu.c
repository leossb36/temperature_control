#include <curses.h>

void menu() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *main = newwin(6, xMax-12, yMax-8, 5);
    box(main, 0, 0);
    refresh();
    keypad(main, true);
}

int main(int argc, char *argv[]) {
    initscr();
    noecho();
    cbreak();

    menu();

    getch();
    endwin();

    return 0;
}