//
// Created by hpae on 06.01.22.
//

#ifndef CCONNECT4_GAME_H
#define CCONNECT4_GAME_H

#include "window.h"
#include <stdint.h>

#define ROWS 6
#define COLUMS 7
#define PLAYER_Y 1
#define PLAYER_R 2

enum gameState {
    QUIT_STATE = 0,
    RUNNING_STATE = 1,
    PLAYER_Y_WON_STATE = 2,
    PLAYER_R_WON_STATE = 3,
    TIE_STATE = 4,
};

enum cellState {
    EMPTY = 0,
    YELLOW = 1,
    RED = 2
};

typedef struct {
    uint8_t board[ROWS][COLUMS];
    uint8_t player;
    uint8_t state;
} game_t;

void clickedOnColumn(game_t *game, int row, int column);

void clickedOnColumn(game_t *game, int row, int column) {

}

#endif //CCONNECT4_GAME_H
