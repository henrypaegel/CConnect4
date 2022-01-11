//
// Created by hpae on 10.01.22.
//

#include "render.h"
#include "logic.h"

const SDL_Color GRID_COLOR = { .r = 0, .g = 50, .b = 130 };
const SDL_Color RED_COLOR = { .r = 180, .g = 0, .b = 0 };
const SDL_Color YELLOW_COLOR = { .r = 230, .g = 180, .b = 20 };
const SDL_Color TIE_COLOR = { .r = 169, .g = 169, .b = 169 };
const SDL_Color WHITE = { .r = 255, .g = 255, .b = 255 };


/* ---IMPLEMENTATIONS--- */

void renderBar(SDL_Renderer *renderer, const SDL_Color *color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    SDL_Rect *board;
    board = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    board->x = 0;
    board->y = 0;
    board->w = SCREEN_WIDTH;
    board->h = CELL_EDGE;
    SDL_RenderFillRect(renderer, board);
    free(board);
} // renders whole background-color

//TODO: streamline rendering of grid; don't re-draw what is already there
void renderGrid(SDL_Renderer *renderer, const SDL_Color *color, const SDL_Color *colorBackground) {
    // set crate color and draw crate
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    SDL_Rect *board;
    board = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    board->x = 0;
    board->y = CELL_EDGE;
    board->w = SCREEN_WIDTH;
    board->h = ROWS*CELL_EDGE;
    SDL_RenderFillRect(renderer, board);
    free(board);

    // iterate through every cell and 'carve out' a white (empty) cell
    for(int i = 1; i <= COLUMNS; i++) {
        for(int j = 1; j <= ROWS; j++) {
            filledCircleRGBA(renderer, i * CELL_EDGE - 0.5 * CELL_EDGE, j * CELL_EDGE + 0.5 * CELL_EDGE, 0.9 * 0.5 * SCREEN_PITCH, colorBackground->r, colorBackground->g, colorBackground->b, 255);
        }
    }
} // renders crate and initializes game with empty (white) cells

//TODO: streamline rendering of colored cells; don't re-draw what is already there
void renderBoard(SDL_Renderer *renderer, const uint8_t board[ROWS][COLUMNS], const SDL_Color *colorPlayerY, const SDL_Color *colorPlayerR) {
    for(int j = 0; j < ROWS; j++) {
        for(int i = 0; i < COLUMNS; i++) {
            switch (board[j][i]) { // get cellState of current cell
                case RED:
                    filledCircleRGBA(renderer, i * CELL_EDGE + 0.5 * CELL_EDGE, j * CELL_EDGE + 1.5 * CELL_EDGE, 0.9 * 0.5 * SCREEN_PITCH, colorPlayerR->r, colorPlayerR->g, colorPlayerR->b, 255);
                    break;

                case YELLOW:
                    filledCircleRGBA(renderer, i * CELL_EDGE + 0.5 * CELL_EDGE, j * CELL_EDGE + 1.5 * CELL_EDGE, 0.9 * 0.5 * SCREEN_PITCH, colorPlayerY->r, colorPlayerY->g, colorPlayerY->b, 255);
                    break;

                default: {}
            }
        }
    }
} // renders cells either as YELLOW, RED or doesn't change them depending on the board-array

void renderRunningState(SDL_Renderer *renderer, const game_t *game) {
    renderGrid(renderer, &GRID_COLOR, &WHITE);
    renderBoard(renderer, game->board, &YELLOW_COLOR, &RED_COLOR);
} // only renders crate and actual colored pieces; background and bar stays untouched

void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color) {
    renderBar(renderer, color);
    renderGrid(renderer, &GRID_COLOR, &TIE_COLOR
    );
    renderBoard(renderer, game->board, &YELLOW_COLOR, &RED_COLOR);
} // renders background, then crate with white pieces and lastly actual colored pieces

void renderGame(SDL_Renderer *renderer, const game_t *game) {
    switch (game->state) {
        case RUNNING_STATE:
            renderRunningState(renderer, game);
            break;

        case RED_WON_STATE:
            renderGameOverState(renderer, game, &RED_COLOR);
            break;

        case YELLOW_WON_STATE:
            renderGameOverState(renderer, game, &YELLOW_COLOR);
            break;

        case TIE_STATE:
            renderGameOverState(renderer, game, &TIE_COLOR);
            break;

        default: {}
    }
} // invoke different renderings with specific colors depending on game-state
