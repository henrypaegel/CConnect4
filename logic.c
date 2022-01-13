//
// Created by hpae on 10.01.22.
//

#include "logic.h"


/* ---IMPLEMENTATIONS--- */

void switchPlayer(game_t *game) {
    if(game->player == RED) {
        game->player = YELLOW;
    } else if(game->player == YELLOW) {
        game->player = RED;
    }
} // set player with next turn to opposite player

int countCells(const uint8_t board[ROWS][COLUMNS], const uint8_t cell) {
    int count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if(board[i][j] == cell) count++; // compare currently viewed cell against reference
        }
    }
    return count;
} // count specific values on the grid

int checkShift(game_t *game, uint8_t player, int r, int c, int rShift, int cShift, int countFour) {
    if(r + rShift < 0 || r + rShift >= ROWS || c + cShift < 0 || c + cShift >= COLUMNS)
        return countFour; // outside grid
    if(countFour < 4) {
        if(game->board[r + rShift][c + cShift] == player) {
            countFour++;
            return checkShift(game, player, r + rShift, c + cShift, rShift, cShift, countFour);
        } else {
            return countFour;
        }
    }
    return countFour;
}

int checkPlayerWon(game_t *game, uint8_t player, cell *newPiece) {
    if(game->board[newPiece->row][newPiece->column] == player) {
        //check along vertical axis
        int countFour = 1;
        countFour = checkShift(game, player, newPiece->row, newPiece->column, -1, 0, countFour);
        if(countFour == 4) return 1;
        countFour = checkShift(game, player, newPiece->row, newPiece->column, 1, 0, countFour);
        if(countFour == 4) return 1;

        //check along horizontal and diagonal axis
        for (int i = -1; i < 2; i++) {
            countFour = 1;
            countFour = checkShift(game, player, newPiece->row, newPiece->column, i, 1, countFour);
            if(countFour == 4) return 1;
            countFour = checkShift(game, player, newPiece->row, newPiece->column, i * -1, -1, countFour);
            if(countFour == 4) return 1;
        }
    }
    return 0;
} // run checks to catch four in a row :)

void gameOverCondition(game_t *game, cell *newPiece) {
    if(checkPlayerWon(game, YELLOW, newPiece)) {
        game->state = YELLOW_WON_STATE;
    } else if(checkPlayerWon(game, RED, newPiece)) {
        game->state = RED_WON_STATE;
    } else if(!countCells(game->board, EMPTY)) { // no empty cells left
        game->state = TIE_STATE;
    }
} // run checks to change game-state if necessary and catch end of game

void playerTurn(game_t *game, cell *newPiece) {
    game->board[newPiece->row][newPiece->column] = game->player; // hard-set new move
    gameOverCondition(game, newPiece); // catch game-state-change
    switchPlayer(game); // prepare next turn
} // player makes a move; new board-layout is checked against game-state-change

void resetGame(game_t *game) {
    game->player = RED; //TODO: eventuell vom Nutzer zu wählen
    game->state = RUNNING_STATE;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            game->board[i][j] = EMPTY;
        }
    }
} // (re)set board as well as other game-attributes to their initial values

void clickedOnColumn(game_t *game, int column, int row) {
    if(game->state == RUNNING_STATE) {
        if(row) return;
        for (int i = ROWS - 1; i >= 0; i--) { // iterate from bottom to top on chosen column
            if(game->board[i][column] == EMPTY) { // check if column has at least one empty cell
                cell newPiece = {.row = i, .column = column};
                playerTurn(game, &newPiece);
                break;
            }
        }
    } else {
        resetGame(game);
    }
} // invoke actions that follow a mousebutton-down-event
