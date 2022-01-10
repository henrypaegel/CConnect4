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
const SDL_Color PLAYER_Y_COLOR = { .r = 255, .g = 255, .b = 0 };
const SDL_Color TIE_COLOR = { .r = 100, .g = 100, .b = 100 };

void drawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);
void filledCircle(SDL_Renderer *renderer, int x, int y, int radius);
void renderGame(SDL_Renderer *renderer, const game_t *game);
void renderGrid(SDL_Renderer *renderer, const SDL_Color *color);
void renderBoard(SDL_Renderer *renderer, const uint8_t **board, const SDL_Color *colorPlayerY, const SDL_Color *colorPlayerR);
void renderRunningState(SDL_Renderer *renderer, const game_t *game);
void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color);

/* ---IMPLEMENTATIONS--- */

void drawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

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

void renderGrid(SDL_Renderer *renderer, const SDL_Color *color) {
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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i = 1; i <= COLUMNS; i++) {
        for(int j = 1; j <= ROWS; j++) {
            filledCircle(renderer, i * CELL_EDGE - 0.5 * CELL_EDGE, j * CELL_EDGE + 0.5 * CELL_EDGE, 0.9 * 0.5 * SCREEN_PITCH);
        }
    }


}

void renderBoard(SDL_Renderer *renderer, const uint8_t **board, const SDL_Color *colorPlayerY, const SDL_Color *colorPlayerR) {

}

void renderRunningState(SDL_Renderer *renderer, const game_t *game) {
    renderGrid(renderer, &GRID_COLOR);
    renderBoard(renderer, game->board, &PLAYER_Y_COLOR, &PLAYER_R_COLOR);

}

void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color) {
    renderGrid(renderer, color);
    renderBoard(renderer, game->board, color, color);
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

        default: {}
    }
}
#endif //CCONNECT4_WINDOW_H
