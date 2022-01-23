//
// Created by hpae on 10.01.22.
//

#include "render.h"
#include "logic.h"

const SDL_Color GRID_COLOR = {.r = 0, .g = 50, .b = 130};
const SDL_Color RED_COLOR = {.r = 180, .g = 0, .b = 0};
const SDL_Color YELLOW_COLOR = {.r = 230, .g = 180, .b = 20};
const SDL_Color TIE_COLOR = {.r = 169, .g = 169, .b = 169};
const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255};


/* ---IMPLEMENTATIONS--- */

void renderColumnHover(SDL_Renderer *renderer, const SDL_Color *color, int column) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    SDL_Rect *board;
    board = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    board->x = CELL_EDGE * column;
    board->y = 0;
    board->w = CELL_EDGE;
    board->h = CELL_EDGE;
    SDL_RenderFillRect(renderer, board);
    free(board);
} // renders whole background-color


int renderHovering(SDL_Renderer *renderer, const int column, const game_t *game) {
    renderBar(renderer, &WHITE, game);
    renderColumnHover(renderer, &TIE_COLOR, column);
    return column;
}

void renderBar(SDL_Renderer *renderer, const SDL_Color *color, const game_t *game) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    SDL_Rect *bar;
    bar = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    bar->x = 0;
    bar->y = 0;
    bar->w = SCREEN_WIDTH;
    bar->h = CELL_EDGE;
    SDL_RenderFillRect(renderer, bar);

    TTF_Font *font = TTF_OpenFont("arial.ttf", 25);
    SDL_Color textColor={0,0,0, 0};
    SDL_Surface *textSurface;

    char moves[10];
    sprintf(moves, "moves: %.2d", movesToWin(game->moves));
    textSurface = TTF_RenderUTF8_Solid(font, moves, textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstRect = { 0, 0, texW, texH };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    /* TODO: some out of scope error causes Stack smashing :-(
    char timer[10];
    double time = getTime();
    sprintf(timer, "time: %.2ds", time);
    textSurface = TTF_RenderUTF8_Solid(font, moves, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    dstRect.w = texW;
    dstRect.h = texH;
    dstRect.x = 0;
    dstRect.y = 10;
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    */

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(textSurface);

    TTF_CloseFont(font);
    free(bar);
} // renders whole background-color

void renderGrid(SDL_Renderer *renderer) {
    SDL_Color color = GRID_COLOR;
    SDL_Color colorBackground = WHITE;
    // set crate color and draw crate
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_Rect *board;
    board = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    board->x = 0;
    board->y = CELL_EDGE;
    board->w = SCREEN_WIDTH;
    board->h = ROWS * CELL_EDGE;
    SDL_RenderFillRect(renderer, board);
    free(board);

    // iterate through every cell and 'carve out' a white (empty) cell
    for (int i = 1; i <= COLUMNS; i++) {
        for (int j = 1; j <= ROWS; j++) {
            filledCircleRGBA(renderer, i * CELL_EDGE - 0.5 * CELL_EDGE, j * CELL_EDGE + 0.5 * CELL_EDGE,
                             0.9 * 0.5 * SCREEN_PITCH, colorBackground.r, colorBackground.g, colorBackground.b, 255);
        }
    }
} // renders crate and initializes game with empty (white) cells

void renderBoard(SDL_Renderer *renderer, const game_t *game) {
    for (int j = 0; j < ROWS; j++) {
        for (int i = 0; i < COLUMNS; i++) {
            switch (game->board[j][i]) { // get cellState of current cell
                case RED:
                    filledCircleRGBA(renderer, i * CELL_EDGE + 0.5 * CELL_EDGE, j * CELL_EDGE + 1.5 * CELL_EDGE,
                                     0.9 * 0.5 * SCREEN_PITCH, RED_COLOR.r, RED_COLOR.g, RED_COLOR.b, 255);
                    break;

                case YELLOW:
                    filledCircleRGBA(renderer, i * CELL_EDGE + 0.5 * CELL_EDGE, j * CELL_EDGE + 1.5 * CELL_EDGE,
                                     0.9 * 0.5 * SCREEN_PITCH, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, 255);

                    break;

                default: {
                }
            }
        }
    }

} // renders new cell either as YELLOW or RED


void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color) {
    renderGrid(renderer);
    renderBoard(renderer, game);
    renderBar(renderer, color, game);
} // renders background, then crate with white pieces and lastly actual colored pieces

void renderGame(SDL_Renderer *renderer, const game_t *game) {
    switch (game->state) {
        case RUNNING_STATE:
            renderBar(renderer, &WHITE, game);
            renderGrid(renderer);
            renderBoard(renderer, game);
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

        default: {
        }
    }
} // invoke different renderings with specific colors depending on game-state
