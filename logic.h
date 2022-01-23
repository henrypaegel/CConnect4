//
// Created by hpae on 10.01.22.
//

#ifndef CCONNECT4_LOGIC_H
#define CCONNECT4_LOGIC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //you need this for linux!



/* ---DEFINITIONS--- */

#define ROWS 6
#define COLUMNS 7
#define TRUE 1
#define FALSE 0

#define AI_GAME 1
#define AI_MODE MEDIUM

//TODO: check whether CELL_EDGE is equivalent to SCREEN_PITCH
#define CELL_EDGE (SCREEN_WIDTH / COLUMNS) // size of square of cells on the board

enum aiMode {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
};

enum gameState {
    QUIT_STATE = 0,
    RUNNING_STATE = 1,
    YELLOW_WON_STATE = 2,
    RED_WON_STATE = 3,
    TIE_STATE = 4,
    MENU_STATE = 5
}; // different game states which invoke specific parts of the program

enum cellState {
    EMPTY = 0,
    YELLOW = 1,
    RED = 2,
}; // all possible states a cell on the board can have

typedef struct {
    uint8_t row;
    uint8_t column;
} cell;

typedef struct {
    double time;
    int moves;
} highscore;

typedef struct {
    uint8_t board[ROWS][COLUMNS];
    cell newPiece;
    uint8_t player;
    uint8_t state;
    uint8_t aiTurn;
    int moves;
} game_t; // game-structure containing current board layout, player with next turn, game state.

int movesToWin(int movesTotal);

void switchPlayer(game_t *game);

int checkPlayerWon(game_t *game, uint8_t player, cell *newPiece);

void playerTurn(game_t *game, cell *newPiece);

void computerTurn(game_t *game);

void resetGame(game_t *game);

int countCells(const uint8_t board[ROWS][COLUMNS], uint8_t cell);

int checkShift(game_t *game, uint8_t player, int r, int c, int rShift, int cShift, int countFour);

void gameOverCondition(game_t *game, cell *newPiece);

void clickedOnColumn(game_t *game, int column, int row);

#endif //CCONNECT4_LOGIC_H
