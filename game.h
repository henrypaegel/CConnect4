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
int checkPlayerWon(game_t *game, uint8_t player);
void playerTurn(game_t *game, int row, int column);
void resetGame(game_t *game);
int countCells(const uint8_t board[ROWS][COLUMNS], uint8_t cell);
void gameOverCondition(game_t *game);
void clickedOnColumn(game_t *game, int column);

/* ---IMPLEMENTATIONS--- */

void switchPlayer(game_t *game) {
    if (game->player == PLAYER_R) {
        game->player = PLAYER_Y;
    } else if (game->player == PLAYER_Y) {
        game->player = PLAYER_R;
    }
}

int countCells(const uint8_t board[ROWS][COLUMNS], uint8_t cell) {
    int count = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            if(board[i][j] == cell) count++;
        }
    }
    return count;
} // count specific values on grid

int checkPlayerWon(game_t *game, uint8_t player) {
    //TODO: implement check for four in row
    return 0;
}

void gameOverCondition(game_t *game) {
    if(checkPlayerWon(game, PLAYER_Y)) {
        game->state = PLAYER_Y_WON_STATE;
    } else if(checkPlayerWon(game, PLAYER_R)) {
        game->state = PLAYER_R_WON_STATE;
    } else if(!countCells(game->board, EMPTY)) {
        game->state = TIE_STATE;
    }
}

void playerTurn(game_t *game, int row, int column) {
    if (game->board[row][column] == EMPTY) {
        game->board[row][column] = game->player;
        switchPlayer(game);
        gameOverCondition(game); //checks if game state changes after move by player
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
        for(int i = ROWS-1; i >= 0; i--) {
            if(game->board[i][column] == EMPTY) {
                playerTurn(game, i, column);
                break;
            }
        }
    } else {
        resetGame(game);
    }
}

#endif //CCONNECT4_GAME_H
