#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "tile.h"

/* main takes command line arguments: playing area (default: 10), bombs (default: 1/5 of total playing area)*/
int main() {
    int size_y = 10;
    int size_x = 10;
    int area = size_y * size_x;
    int bomb_number = (int)(area / 10);
    tile ** map;

    /* starts ncurses */
    initscr();
    noecho();

    /* sets up mouse input */

    /* starts randomization */
    srand(time(NULL));

    /* sets up game */
    map = createTileMap(size_y, size_x);
    setBombs(map, size_y, size_x, bomb_number);
    setValues(map, size_y, size_x);

    char player_input;
    int x_in, y_in;

    /* game loop */
    do{
        debugMap(map, size_y, size_x);
        mvprintw(size_y + 2, 2, "x: "); refresh(); x_in = getch() - '0';
        mvprintw(size_y + 2, 2, "y: "); refresh(); y_in = getch() - '0';

        revealTiles(map, size_y, size_x, y_in, x_in);
        printMap(map, size_y, size_x);
    } while(x_in != 'q' - '0' || y_in != 'q' - '0');

    /* closes ncurses window */
    endwin();
    /* frees map memory */
    free(map);
}