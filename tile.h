#include <stdio.h>
#include <stdlib.h>

typedef struct _tile {
    int value;
    bool is_bomb;
    bool is_flag;
    bool is_hidden;
} tile;

tile ** createTileMap(int rows, int cols);
void printMap(tile ** map, int rows, int cols, int print_option);
void debugMap(tile ** map, int rows, int cols);

bool openTile(tile ** map, int rows, int cols, int y, int x);
bool revealTiles(tile ** map, int rows, int cols, int y, int x);
bool chord(tile ** map, int rows, int cols, int y, int x);
bool canChord(tile ** map, int rows, int cols, int y, int x);
void putFlag(tile ** map, int rows, int cols, int y, int x);

bool checkWin(tile ** map, int rows, int cols);

void setBombs(tile ** map, int rows, int cols, int safe_y, int safe_x, int bomb_number);
void setValues(tile ** map, int rows, int cols);

int searchBombs(tile ** map, int rows, int cols, int y, int x);
int searchFlags(tile ** map, int rows, int cols, int y, int x);
