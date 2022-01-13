//
// Created by hpae on 02.01.22.
//

#include "logic.h"
#include "render.h"


int main(int argc, char **argv) {
    if(SDL_Init(SDL_INIT_VIDEO)) {
        printf("Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } // initialize SDL2 subsystem with video support

    SDL_Window *window = SDL_CreateWindow("CConnect4", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } // create SDL2 window object and catch possible exceptions

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } // create SDL2 renderer object and catch possible exceptions


    // create game object and initialize the game
    game_t *game = (game_t *) malloc(sizeof(game_t));
    resetGame(game);
    // set everything to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);


    // game loop driven by SDL_Event to react on user input
    SDL_Event e;
    while (game->state) {
        int prev = -1;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    game->state = QUIT_STATE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    clickedOnColumn(game, e.button.x / CELL_EDGE, e.button.y / CELL_EDGE);
                    break;

                case SDL_MOUSEMOTION:
                    if(!(e.button.y / CELL_EDGE)) {
                        if(prev != (e.button.y / CELL_EDGE)) prev = renderHovering(renderer, e.button.x / CELL_EDGE);
                    }

                default: {
                }
            }
        }

        // render new game and present to user
        renderGame(renderer, game);
        SDL_RenderPresent(renderer);
    }


    //quit and deallocate everything
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(game);

    return EXIT_SUCCESS;
}
