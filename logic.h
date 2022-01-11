//
// Created by hpae on 10.01.22.
//

#ifndef CCONNECT4_LOGIC_H
#define CCONNECT4_LOGIC_H

#include <stdint.h>


/* ---DEFINITIONS--- */

#define ROWS 6
#define COLUMNS 7

//TODO: check whether CELL_EDGE is equivalent to SCREEN_PITCH
#define CELL_EDGE (SCREEN_WIDTH / COLUMNS) // size of square of cells on the board

enum gameState {
    QUIT_STATE = 0,
    RUNNING_STATE = 1,
    YELLOW_WON_STATE = 2,
    RED_WON_STATE = 3,
    TIE_STATE = 4,
}; // different game states which invoke specific parts of the program

enum cellState {
    EMPTY = 0,
    YELLOW = 1,
    RED = 2
}; // all possible states a cell on the board can have

typedef struct {
    uint8_t board[ROWS][COLUMNS];
    uint8_t player;
    uint8_t state;
} game_t; // game-structure containing current board layout, player with next turn, game state.

typedef struct {
    uint8_t row;
    uint8_t column;
} cell;

void switchPlayer(game_t *game);
int checkPlayerWon(game_t *game, uint8_t player, cell *newPiece);
void playerTurn(game_t *game, cell *newPiece);
void resetGame(game_t *game);
int countCells(const uint8_t board[ROWS][COLUMNS], uint8_t cell);
void gameOverCondition(game_t *game, cell *newPiece);
void clickedOnColumn(game_t *game, int column);

#endif //CCONNECT4_LOGIC_H
