#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "tile.h"

/* main takes command line arguments */
int main(int argc, char * argv[]) {
    /* default values */
    int size_y = 16;
    int size_x = 30;

    /* checks for more cl arguments */
    if(argc > 1) {
        if(argc == 3) {
            size_x = atoi(argv[1]);
            size_y = atoi(argv[2]);
        }
    }
    
    int area = size_y * size_x;
    int bomb_number = (int)(area / 4.8) - 1;
    tile ** map;
    bool has_lost = false;
    time_t start, end;

    /* starts ncurses */
    WINDOW * main_win = initscr();
    noecho();
    cbreak();
    keypad(main_win, true);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    MEVENT mouse_input;

    /* colors */
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_BLACK, COLOR_RED); // flags

    /* sets up mouse input */

    /* starts randomization */
    srand(time(NULL));

    /* sets up game */
    map = createTileMap(size_y, size_x);
    bool is_map_gen = false;

    int player_input;
    int y_pos = 0;
    int x_pos = 0;

    /* game loop */
    do{
        printMap(map, size_y, size_x, 0);
        move(y_pos, x_pos);
        player_input = getch();

        switch(player_input) {

            /* move cursor */
            case '8': case KEY_UP: y_pos--; break;
            case '2': case KEY_DOWN: y_pos++; break;
            case '4': case KEY_LEFT: x_pos--; break;
            case '6': case KEY_RIGHT: x_pos++; break;
            case '7': y_pos--; x_pos--; break;
            case '1': y_pos++; x_pos--; break;
            case '9': y_pos--; x_pos++; break;
            case '3': y_pos++; x_pos++; break;

            case 'z': case 'Z':
            if(is_map_gen == false) {
                /* creates map after first click */
                setBombs(map, size_y, size_x, y_pos, x_pos, bomb_number);
                setValues(map, size_y, size_x);

                is_map_gen = true;

                /* gets starting time */
                time(&start);
            }
            has_lost = openTile(map, size_y, size_x, y_pos, x_pos); break;

            case 'x': case 'X':
            putFlag(map, size_y, size_x, y_pos, x_pos); break;

            case KEY_MOUSE:
             if(getmouse(& mouse_input) == OK) {
                y_pos = mouse_input.y;
                x_pos = mouse_input.x;

                /* check if position is valid */
                if(y_pos < 0) y_pos = 0;
                else if(y_pos >= size_y) y_pos = size_y - 1;
                if(x_pos < 0) x_pos = 0;
                else if(x_pos >= size_x) x_pos = size_x - 1;

                if(mouse_input.bstate & BUTTON1_CLICKED) {
                    if(is_map_gen == false) {
                        /* creates map after first click */
                        setBombs(map, size_y, size_x, y_pos, x_pos, bomb_number);
                        setValues(map, size_y, size_x);

                        is_map_gen = true;

                        /* gets starting time */
                        time(&start);
                    }
                  has_lost = openTile(map, size_y, size_x, y_pos, x_pos); break;
                }

                if(mouse_input.bstate & BUTTON3_CLICKED) {
                    putFlag(map, size_y, size_x, y_pos, x_pos); break;
                }
             }
        }

        /* check if movement is valid */
        if(y_pos < 0) y_pos++;
        else if(y_pos >= size_y) y_pos--;
        if(x_pos < 0) x_pos++;
        else if(x_pos >= size_x) x_pos--;

        if(has_lost == true) {
            time(&end);
            do {
            mvprintw(size_y + 2, 2, "You lost in %ld seconds! Press q to exit.", end - start);
            printMap(map, size_y, size_x, 1);
            player_input = getch();
            } while(player_input != 'q' && player_input != 'Q');

            break;
        }

        if(checkWin(map, size_y, size_x) == true) {
            time(&end);
            do {
            printMap(map, size_y, size_x, 0);
            mvprintw(size_y + 2, 2, "You won in %ld seconds!! Press q to exit.", end - start);
            player_input = getch();
            } while(player_input != 'q' && player_input != 'Q');

            break;
        }

    } while(player_input != 'q' && player_input != 'Q');

    /* closes ncurses window */
    endwin();
    /* frees map memory */
    free(map);
}
