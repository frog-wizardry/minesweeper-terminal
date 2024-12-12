#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "tile.h"

/* main takes command line arguments: playing area (default: 10), bombs (default: 1/5 of total playing area)*/
int main() {
    int size_y = 20;
    int size_x = 50;
    int area = size_y * size_x;
    int bomb_number = (int)(area / 6);
    tile ** map;

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
        printMap(map, size_y, size_x);
        mvprintw(2, size_x + 2, "%d, %d", y_pos, x_pos);
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
                setBombs(map, size_y, size_x, y_pos, x_pos, bomb_number);
                setValues(map, size_y, size_x);
                is_map_gen = true;
            }
            openTile(map, size_y, size_x, y_pos, x_pos); break;

            case 'x': case 'X':
            putFlag(map, size_y, size_x, y_pos, x_pos); break;

            case KEY_MOUSE:
             if(getmouse(& mouse_input) == OK) {
                if(mouse_input.bstate & BUTTON1_CLICKED) {
                    if(is_map_gen == false) {
                        setBombs(map, size_y, size_x, mouse_input.y, mouse_input.x, bomb_number);
                        setValues(map, size_y, size_x);
                        is_map_gen = true;
                    }
                    openTile(map, size_y, size_x, mouse_input.y, mouse_input.x); break;
                }

                if(mouse_input.bstate & BUTTON3_CLICKED) {
                    putFlag(map, size_y, size_x, mouse_input.y, mouse_input.x); break;
                }
             }
        }
    } while(player_input != 'q' && player_input != 'Q');

    /* closes ncurses window */
    endwin();
    /* frees map memory */
    free(map);
}