//
// Created by hpae on 06.01.22.
//

#ifndef CCONNECT4_WINDOW_H
#define CCONNECT4_WINDOW_H

#include "game.h"
#include <SDL2/SDL.h>


/* ---DEFINITIONS--- */

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void renderGame(SDL_Renderer *renderer, const game_t *game);
void renderGrid(SDL_Renderer *renderer, const SDL_Color *color);
void renderBoard(SDL_Renderer *renderer, const int **board, const SDL_Color *colorPlayerY, const SDL_Color *colorPlayerR);
void renderRunningState(SDL_Renderer *renderer, const game_t *game);
void renderGameOverState(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color);

/* ---IMPLEMENTATIONS--- */


#endif //CCONNECT4_WINDOW_H
