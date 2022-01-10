//
// Created by hpae on 06.01.22.
//

#ifndef CCONNECT4_GAME_H
#define CCONNECT4_GAME_H

#include "window.h"
#include <stdint.h>


/* ---DEFINITIONS--- */

#define ROWS 6
#define COLUMNS 7
#define PLAYER_Y 1
#define PLAYER_R 2
#define CELL_EDGE (SCREEN_WIDTH / COLUMNS)

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
    uint8_t board[ROWS][COLUMNS];
    uint8_t player;
    uint8_t state;
} game_t;

void switchPlayer(game_t *game);
void playerTurn(game_t *game, int row, int column);
void resetGame(game_t *game);
void clickedOnColumn(game_t *game, int column);

/* ---IMPLEMENTATIONS--- */

void switchPlayer(game_t *game) {
    if (game->player == PLAYER_R) {
        game->player = PLAYER_Y;
    } else if (game->player == PLAYER_Y) {
        game->player = PLAYER_R;
    }
}

void playerTurn(game_t *game, int row, int column) {
    if (game->board[row][column] == EMPTY) {
        game->board[row][column] = game->player;
        switchPlayer(game);
    }
}

void resetGame(game_t *game) {
    game->player = PLAYER_R;
    game->state = RUNNING_STATE;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            game->board[i][j] = EMPTY;
        }
    }
}

void clickedOnColumn(game_t *game, int column) {
    if(game->state == RUNNING_STATE) {
        int validTurn = 0;
        for(int i = ROWS-1; i >= 0; i--) {
            if(game->board[i][column] == EMPTY) {
                playerTurn(game, i, column);
                validTurn = 1;
                break;
            }
        }
        if(!validTurn) {
            // aktuell noch Pech :P
        }
    } else {
        resetGame(game);
    }
}

#endif //CCONNECT4_GAME_H
