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

void catchPollEvent(SDL_Event e, int* quit);
void renderGame(SDL_Renderer *renderer, const game_t *game);


/* ---IMPLEMENTATIONS--- */

void catchPollEvent(SDL_Event e, int* quit) {
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                *quit = 1;
                break;

            default: {
            }
        }
    }
}

void renderGame(SDL_Renderer *renderer, const game_t *game) {

}

#endif //CCONNECT4_WINDOW_H
