#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "tile.h"

tile ** createTileMap(int rows, int cols) {
    tile ** out_map;

    /* allocates memory for the map */
    out_map = (tile **)malloc(rows * sizeof(tile *));
    for(int i = 0; i < rows; i++) {
        out_map[i] = (tile *)malloc(cols * sizeof(tile));
    }

    /* sets default values for all tiles */
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            out_map[i][j].value = 0;
            out_map[i][j].is_bomb = false;
            out_map[i][j].is_flag = false;
            out_map[i][j].is_hidden = true;
        }
    }

    return out_map;
}

void printMap(tile ** map, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {

            /* check tile in position */
            if(map[i][j].is_flag == true) {
                mvaddch(i, j, '!');
            }

            else if(map[i][j].is_hidden == true) {
                mvaddch(i, j, '-');
            }

            else if(map[i][j].is_bomb == true) {
                mvaddch(i, j, '*');
            }

            else if(map[i][j].value == 0) {
                mvaddch(i, j, ' ');
            }

            else mvaddch(i, j, map[i][j].value + '0');
        }
    }

    refresh();
}

void debugMap(tile ** map, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {

            if(map[i][j].is_hidden == true) {
                attron(A_DIM);
            }

            if(map[i][j].is_flag == true) {
                mvaddch(i, j, '!');
            }

            else if(map[i][j].is_bomb == true) {
                mvaddch(i, j, '*');
            }

            else mvaddch(i, j, map[i][j].value + '0');

            attroff(A_DIM);
        }
    }

    refresh();
    getch();
}

void revealTiles(tile ** map, int rows, int cols, int y, int x) {
    /* if tile is not in map */
    if(y < 0 || y >= rows || x < 0 || x >= cols) {
        return;
    }

    /* if tile is already revealed */
    if(map[y][x].is_hidden == false) {
        return;
    }

    map[y][x].is_hidden = false;

    /* if value is 0 */
    if(map[y][x].value == 0) {
        for(int i = -1; i < 2; i++) {
            for(int j = -1; j < 2; j++) {
                revealTiles(map, rows, cols, y + i, x + j);
            }
        }
    }
}

void setBombs(tile ** map, int rows, int cols, int bomb_number) {
    int rand_y, rand_x;
    

    /* loop for every bomb */
    for(int i = 0; i < bomb_number; i++) {        
        rand_y = rand() % rows; rand_x = rand() % cols;

        /* if tile chosen has no bomb, place one there */
        if(map[rand_y][rand_x].is_bomb == false) {
            map[rand_y][rand_x].is_bomb = true;
            map[rand_y][rand_x].value = -1;
        }

        /* else, try again */
        else {
            i--;
        }
    }
}

void setValues(tile ** map, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(map[i][j].is_bomb == false) {
                map[i][j].value = searchBombs(map, rows, cols, i, j);
            }
        }
    }
}

int searchBombs(tile ** map, int rows, int cols, int y, int x) {
    int i_start, i_end, j_start, j_end;
    int bombs_found = 0;

    i_start = y - 1; i_end = y + 1;
    j_start = x - 1; j_end = x + 1;

    /* check if y value is close to the border */
    if(i_start < 0) {
        i_start = 0;
    }

    if(i_end >= rows) {
        i_end = rows - 1;
    }

    /* check same for x */
    if(j_start < 0) {
        j_start = 0;
    }

    if(j_end >= cols) {
        j_end = cols - 1;
    }

    /* check nearby tiles */
    for(int i = i_start; i <= i_end; i++) {
        for(int j = j_start; j <= j_end; j++) {
            if(map[i][j].is_bomb == true) {
                bombs_found++;
            }
        }
    }

    return bombs_found;
}