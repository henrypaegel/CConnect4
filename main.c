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
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // swap with function to reset game;
    game_t *game = (game_t*) malloc(sizeof(game_t));
    resetGame(game);


    SDL_Event e;
    while (game->state) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    game->state = QUIT_STATE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    clickedOnColumn(game, e.button.x / CELL_EDGE);
                    break;

                default: {}
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        renderGame(renderer, game);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    free(game);

    return EXIT_SUCCESS;
}