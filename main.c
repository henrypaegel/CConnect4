//
// Created by hpae on 02.01.22.
//

#include "window.h"
#include "game.h"


int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("CConnect4", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == 0) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    game_t game = {
        .board = {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                  {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                  {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                  {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                  {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                  {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
        .player = PLAYER_Y,
        .state = RUNNING_STATE
    };


    const float columnWidth = SCREEN_WIDTH / COLUMS;
    const float columnHeight = SCREEN_HEIGHT;

    SDL_Event e;
    while (game.state) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    game.state = QUIT_STATE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    clickedOnColumn(&game, e.button.y / columnHeight, e.button.x / columnWidth);
                    break;

                default: {}
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderGame(renderer, &game);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}