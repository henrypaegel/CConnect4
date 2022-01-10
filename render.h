//
// Created by hpae on 10.01.22.
//

#ifndef CCONNECT4_RENDER_H
#define CCONNECT4_RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "logic.h"


/* ---DEFINITIONS--- */

#define SCREEN_PITCH 100 // how many pixels a CEll_EDGE occupies on screen
#define SCREEN_WIDTH (SCREEN_PITCH * COLUMNS)
#define SCREEN_HEIGHT (SCREEN_PITCH * (ROWS+1))
extern const SDL_Color GRID_COLOR;
extern const SDL_Color RED_COLOR;
extern const SDL_Color YELLOW_COLOR;
extern const SDL_Color TIE_COLOR;
extern const SDL_Color WHITE;

void renderBar(SDL_Renderer *renderer, const SDL_Color *color);
void renderGame(SDL_Renderer *renderer, const game_t *game);
void renderGrid(SDL_Renderer *renderer, const SDL_Color *color, const SDL_Color *colorBackground);
void renderBoard(SDL_Renderer *renderer, const uint8_t board[ROWS][COLUMNS], const SDL_Color *colorPlayerY, const SDL_Color *colorPlayerR);
void renderRunningState(SDL_Renderer *renderer, const game_t *game);
void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color);

#endif //CCONNECT4_RENDER_H
