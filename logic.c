//
// Created by hpae on 10.01.22.
//

#include "logic.h"

time_t start, end;


/* ---IMPLEMENTATIONS--- */

double getTime() {
    time(&end);
    return difftime(end, start);
}

int movesToWin(int movesTotal) {
    int overflow = movesTotal % 2;
    movesTotal /= 2;
    return movesTotal + overflow;
}

int oppositePlayer(uint8_t player) {
    if(player == 1) return 2;
    if(player == 2) return 1;
    return player;
}

void switchPlayer(game_t *game) {
    if(game->player == RED) {
        game->player = YELLOW;
    } else if(game->player == YELLOW) {
        game->player = RED;
    }

    if(AI_GAME && game->aiTurn) {
        game->aiTurn = FALSE;
    } else if(AI_GAME) {
        game->aiTurn = TRUE;
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
        time(&end);
        printf("took yellow %fs and %dmoves\n", difftime(end, start), movesToWin(game->moves));
    } else if(checkPlayerWon(game, RED, newPiece)) {
        game->state = RED_WON_STATE;
        time(&end);
        printf("took red %fs and %dmoves\n", difftime(end, start), movesToWin(game->moves));
    } else if(!countCells(game->board, EMPTY)) { // no empty cells left
        game->state = TIE_STATE;
        time(&end);
        printf("tie after %fs and %dmoves\n", difftime(end, start), movesToWin(game->moves));
    }
} // run checks to change game-state if necessary and catch end of game

int findEmptyRow(game_t* game, int column) {
    for (int i = ROWS - 1; i >= 0; i--) { // iterate from bottom to top on chosen column
        if(game->board[i][column] == EMPTY) return i;
    }
    return -1;
}

int countInWindow(const uint8_t window[], const uint8_t cell, int offset) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if(window[i+offset] == cell) count++; // compare currently viewed cell against reference
    }
    return count;
}

int scoreAxis(int length, uint8_t axis[length], uint8_t player) {
    // Look through every window of four in every row and assign scores to different formations.
    int score = 0;
    for (int offset = 0; offset < length - 3; offset++) {
        if(countInWindow(axis, player, offset) == 4) {
            score += 100;
        } else if(countInWindow(axis, player, offset) == 3 && countInWindow(axis, EMPTY, offset) == 1) {
            score += 5;
        } else if(countInWindow(axis, player, offset) == 2 && countInWindow(axis, EMPTY, offset) == 2) {
            score += 2;
        } else if(countInWindow(axis, oppositePlayer(player), offset) == 4) {
            score -= 80;
        } else if(countInWindow(axis, oppositePlayer(player), offset) == 3 && countInWindow(axis, EMPTY, offset) == 1) {
            score -= 10;
        }
    }
    return score;
}

void getDiagonal(const uint8_t board[][COLUMNS], uint8_t window[], int startR, int startC) {
    for (int i = 0; i < 6; i++) {
        if(startR+i < 0 || startR+i >= ROWS || startC+i < 0 || startC+i >= COLUMNS) {
            window[i] = -1;
        } else {
            window[i] = board[startR+i][startC+i];
        }
    }
}

int scoreBoard(const uint8_t board[][COLUMNS], cell piece, uint8_t player) {
    int score = 0;

    //center column
    uint8_t center[ROWS];
    for (int r = 0; r < ROWS; r++) {
        // fill row-array with data from game and pretend testMove was done.
        if(piece.row == r && piece.column == 3) {
            center[r] = player;
        } else {
            center[r] = board[r][3];
        }
        // Look through every window of four in every row and assign scores to different formations.
    }
    score += countInWindow(center, player, 0) * 3;


    //check horizontal score
    uint8_t row[COLUMNS];
    for (int r = 0; r < ROWS; r++) {
        // fill row-array with data from game and pretend testMove was done.
        for (int i = 0; i < COLUMNS; i++) {
            if(piece.row == r && piece.column == i) {
                row[i] = player;
            } else {
                row[i] = board[r][i];
            }
        }
        // Look through every window of four in every row and assign scores to different formations.
        score += scoreAxis(COLUMNS, row, player);
    }

    //check vertical score
    uint8_t column[ROWS];
    for (int c = 0; c < COLUMNS; c++) {
        // fill column-array with data from game and pretend testMove was done.
        for (int i = 0; i < ROWS; i++) {
            if(piece.row == i && piece.column == c) {
                column[i] = player;
            } else {
                column[i] = board[i][c];
            }
        }
        // Look through every window of four in every row and assign scores to different formations.
        score += scoreAxis(ROWS, column, player);
    }

    //check diagonal score
    uint8_t diagonal[6];

    // left-leaning (negative slope)
    for (int r = 2; r >= 0; r--) {
        getDiagonal(board, diagonal, r, 0);
        score += scoreAxis(6, diagonal, player);
    }
    for (int c = 0; c <= 3; c++) {
        getDiagonal(board, diagonal, 0, c);
        score += scoreAxis(6, diagonal, player);
    }
    // right-leaning (positive slope)
    for (int c = 3; c <= 6; c++) {
        getDiagonal(board, diagonal, 0, c);
        score += scoreAxis(6, diagonal, player);
    }
    for (int r = 1; r <= 2; r++) {
        getDiagonal(board, diagonal, r, 6);
        score += scoreAxis(6, diagonal, player);
    }


    return score;
}

int randomLegalColumn(const game_t *game) {
    int row = -1;
    int column;
    while(row == -1) {
        column = rand()%6;
        row = findEmptyRow(game, column);
    }
    return column;
}


int pickBestColumn(const game_t *game, const uint8_t player) {
    int bestScore = 0;
    int bestColumn = randomLegalColumn(game);

    for (int i = 0; i < COLUMNS; i++) {
        for (int j = ROWS - 1; j >= 0; j--) { // iterate from bottom to top on chosen column
            int score;
            if(game->board[j][i] == EMPTY) { // check if column has at least one empty cell
                cell testPiece = {.row = j, .column = i};
                score = scoreBoard(game->board, testPiece, player);
                if(score > bestScore) {
                    bestScore = score;
                    bestColumn = i;
                }
                break;
            }
        }
    }
    return bestColumn;
}

int isLastNode(game_t *game, cell *newPiece) {
    return  checkPlayerWon(game, YELLOW, newPiece) ||
            checkPlayerWon(game, RED, newPiece) ||
            !countCells(game->board, EMPTY);
}

void minimax(game_t *game, int depth, int alpha, int beta, uint8_t maximizingPlayer) {
    //int lastNode = isLastNode(game, );
}


void computerTurn(game_t *game) {
    if(AI_MODE == EASY) {
        int column = randomLegalColumn(game);
        cell newPiece = {.row = findEmptyRow(game, column), .column = column};
        playerTurn(game, &newPiece);
    } else if(AI_MODE == MEDIUM) {
        int column = pickBestColumn(game, game->player);
        int row = findEmptyRow(game, column);
        cell newPiece = {.row = row, .column = column};
        playerTurn(game, &newPiece);
    }
}

void makeMove(game_t *game, cell *newPiece) {
    game->board[newPiece->row][newPiece->column] = game->player; // hard-set new move
    game->newPiece = *newPiece;
    game->moves++;
}

void playerTurn(game_t *game, cell *newPiece) {
    makeMove(game, newPiece);
    gameOverCondition(game, newPiece); // catch game-state-change
    switchPlayer(game); // prepare next turn
} // player makes a move; new board-layout is checked against game-state-change

void resetGame(game_t *game) {
    game->player = YELLOW; //TODO: eventuell vom Nutzer zu wählen
    game->state = RUNNING_STATE;
    game->aiTurn = FALSE; //TODO: let user decide if game starts with ai or not (also assigns color to ai)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            game->board[i][j] = EMPTY;
        }
    }
    if(game->aiTurn) computerTurn(game);
    game->moves = 0;
    time(&start);
} // (re)set board as well as other game-attributes to their initial values

void clickedOnColumn(game_t *game, int column, int row) {
    if(game->state == RUNNING_STATE) {
        if(row) return;
        for (int i = ROWS - 1; i >= 0; i--) { // iterate from bottom to top on chosen column
            if(game->board[i][column] == EMPTY) { // check if column has at least one empty cell
                cell newPiece = {.row = i, .column = column};
                if(!game->aiTurn)  {
                    playerTurn(game, &newPiece);
                }
                break;
            }
        }
    } else {
        resetGame(game);
    }
} // invoke actions that follow a mousebutton-down-event
