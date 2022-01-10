//
// Created by hpae on 06.01.22.
//

#ifndef CCONNECT4_WINDOW_H
#define CCONNECT4_WINDOW_H

#include "game.h"
#include <SDL2/SDL.h>


/* ---DEFINITIONS--- */

#define SCREEN_PITCH 100
#define SCREEN_WIDTH (SCREEN_PITCH * COLUMNS)
#define SCREEN_HEIGHT (SCREEN_PITCH * (ROWS+1))
const SDL_Color GRID_COLOR = { .r = 0, .g = 50, .b = 130 };
const SDL_Color PLAYER_R_COLOR = { .r = 180, .g = 0, .b = 0 };
const SDL_Color PLAYER_Y_COLOR = { .r = 230, .g = 180, .b = 20 };
const SDL_Color TIE_COLOR = { .r = 169, .g = 169, .b = 169 };
const SDL_Color WHITE = { .r = 255, .g = 255, .b = 255 };

void filledCircle(SDL_Renderer *renderer, int x, int y, int radius);
void renderBar(SDL_Renderer *renderer, const SDL_Color *color);
void renderGame(SDL_Renderer *renderer, const game_t *game);
void renderGrid(SDL_Renderer *renderer, const SDL_Color *color, const SDL_Color *colorBackground);
void renderBoard(SDL_Renderer *renderer, const uint8_t board[ROWS][COLUMNS], const SDL_Color *colorPlayerY, const SDL_Color *colorPlayerR);
void renderRunningState(SDL_Renderer *renderer, const game_t *game);
void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color);

/* ---IMPLEMENTATIONS--- */

void filledCircle(SDL_Renderer *renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void renderBar(SDL_Renderer *renderer, const SDL_Color *color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    SDL_RenderClear(renderer);
    /*
    SDL_Rect *board;
    board = (SDL_Rect*) malloc(sizeof(board));
    board->x = 0;
    board->y = 0;
    board->w = SCREEN_WIDTH;
    board->h = CELL_EDGE;

    SDL_RenderDrawRect(renderer, board);
    SDL_RenderFillRect(renderer, board);
    free(board);
    */
}

void renderGrid(SDL_Renderer *renderer, const SDL_Color *color, const SDL_Color *colorBackground) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    SDL_Rect *board;
    board = (SDL_Rect*) malloc(sizeof(board));
    board->x = 0;
    board->y = CELL_EDGE;
    board->w = SCREEN_WIDTH;
    board->h = ROWS*CELL_EDGE;

    SDL_RenderDrawRect(renderer, board);
    SDL_RenderFillRect(renderer, board);
    free(board);

    SDL_SetRenderDrawColor(renderer, colorBackground->r, colorBackground->g, colorBackground->b, 255);
    for(int i = 1; i <= COLUMNS; i++) {
        for(int j = 1; j <= ROWS; j++) {
            filledCircle(renderer, i * CELL_EDGE - 0.5 * CELL_EDGE, j * CELL_EDGE + 0.5 * CELL_EDGE, 0.9 * 0.5 * SCREEN_PITCH);
        }
    }
}

void renderBoard(SDL_Renderer *renderer, const uint8_t board[ROWS][COLUMNS], const SDL_Color *colorPlayerY, const SDL_Color *colorPlayerR) {
    for(int j = 0; j < ROWS; j++) {
        for(int i = 0; i < COLUMNS; i++) {
            switch (board[j][i]) {
                case PLAYER_R:
                    //renderR(renderer, i, j, colorPlayerR);
                    SDL_SetRenderDrawColor(renderer, colorPlayerR->r, colorPlayerR->g, colorPlayerR->b, 255);
                    filledCircle(renderer, i * CELL_EDGE + 0.5 * CELL_EDGE, j * CELL_EDGE + 1.5 * CELL_EDGE, 0.9 * 0.5 * SCREEN_PITCH);
                    break;

                case PLAYER_Y:
                    //renderY(renderer, i, j, colorPlayerY);
                    SDL_SetRenderDrawColor(renderer, colorPlayerY->r, colorPlayerY->g, colorPlayerY->b, 255);
                    filledCircle(renderer, i * CELL_EDGE + 0.5 * CELL_EDGE, j * CELL_EDGE + 1.5 * CELL_EDGE, 0.9 * 0.5 * SCREEN_PITCH);
                    break;

                default: {}
            }
        }
    }
}

void renderRunningState(SDL_Renderer *renderer, const game_t *game) {
    renderGrid(renderer, &GRID_COLOR, &WHITE);
    renderBoard(renderer, game->board, &PLAYER_Y_COLOR, &PLAYER_R_COLOR);

}

void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color) {
    renderBar(renderer, color);
    renderGrid(renderer, &GRID_COLOR, color);
    renderBoard(renderer, game->board, &PLAYER_Y_COLOR, &PLAYER_R_COLOR);
}

void renderGame(SDL_Renderer *renderer, const game_t *game) {
    switch (game->state) {
        case RUNNING_STATE:
            renderRunningState(renderer, game);
            break;

        case PLAYER_R_WON_STATE:
            renderGameOverState(renderer, game, &PLAYER_R_COLOR);
            break;

        case PLAYER_Y_WON_STATE:
            renderGameOverState(renderer, game, &PLAYER_Y_COLOR);
            break;

        case TIE_STATE:
            renderGameOverState(renderer, game, &TIE_COLOR);
            break;

        default: {}
    }
}
#endif //CCONNECT4_WINDOW_H
