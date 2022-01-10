//
// Created by hpae on 10.01.22.
//

#include "logic.h"


/* ---IMPLEMENTATIONS--- */

void switchPlayer(game_t *game) {
    if (game->player == RED) {
        game->player = YELLOW;
    } else if (game->player == YELLOW) {
        game->player = RED;
    }
} // set player with next turn to opposite player

int countCells(const uint8_t board[ROWS][COLUMNS], const uint8_t cell) {
    int count = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            if(board[i][j] == cell) count++; // compare currently viewed cell against reference
        }
    }
    return count;
} // count specific values on the grid

int checkPlayerWon(game_t *game, uint8_t player) {
    //TODO: implement check for four in row
    return 0;
} // run checks to catch four in a row :)

void gameOverCondition(game_t *game) {
    if(checkPlayerWon(game, YELLOW)) {
        game->state = YELLOW_WON_STATE;
    } else if(checkPlayerWon(game, RED)) {
        game->state = RED_WON_STATE;
    } else if(!countCells(game->board, EMPTY)) { // no empty cells left
        game->state = TIE_STATE;
    }
} // run checks to change game-state if necessary and catch end of game

void playerTurn(game_t *game, int row, int column) {
    if (game->board[row][column] == EMPTY) {
        game->board[row][column] = game->player; // hard-set new move
        switchPlayer(game); // prepare next turn
        gameOverCondition(game); // catch game-state-change
    }
} // player makes a move; new board-layout is checked against game-state-change

void resetGame(game_t *game) {
    game->player = RED;
    game->state = RUNNING_STATE;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            game->board[i][j] = EMPTY;
        }
    }
} // (re)set board as well as other game-attributes to their initial values

void clickedOnColumn(game_t *game, int column) {
    if(game->state == RUNNING_STATE) {
        for(int i = ROWS-1; i >= 0; i--) { // iterate from bottom to top on chosen column
            if(game->board[i][column] == EMPTY) { // check if column has at least one empty cell
                playerTurn(game, i, column);
                break;
            }
        }
    } else {
        resetGame(game);
    }
} // invoke actions that follow a mousebutton-down-event
