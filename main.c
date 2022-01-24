//
// Created by hpae on 02.01.22.
//

#include "logic.h"
#include "render.h"


int clickedButton(SDL_Event *e) {
    if(e->button.x >= 240 && e->button.x <= 460) {
        if(e->button.y >= 232 && e->button.y <= 292) {
            return 1;
        } else if(e->button.y >= 324 && e->button.y <= 384) {
            return 2;
        } else if(e->button.y >= 416 && e->button.y <= 476) {
            return 3;
        } else if(e->button.y >= 508 && e->button.y <= 568) {
            if(e->button.x <= 348) {
                return 4;
            } else if (e->button.x >= 352) {
                return 5;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    if(SDL_Init(SDL_INIT_VIDEO)) {
        printf("Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } // initialize SDL2 subsystem with video support

    if(TTF_Init() == -1) {
        printf("Could not initialize sdl_ttf: %s\n", TTF_GetError());
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
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // create game object and initialize the game
    game_t *game = (game_t *) malloc(sizeof(game_t));
    game->state = MENU_STATE;

    gameSettings *settings = (gameSettings *) malloc(sizeof(gameSettings));
    settings->aiGame = 1;
    settings->difficulty = MEDIUM;
    settings->randomStart = 0;

    //resetGame(game);
    // set everything to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    renderGrid(renderer);
    renderMenu(renderer, settings);
    SDL_RenderPresent(renderer);


    srand(time(NULL));   // Initialization, should only be called once.

    // game loop driven by SDL_Event to react on user input
    SDL_Event e;
    while (game->state) {
        //int prev = -1;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    game->state = QUIT_STATE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    //TODO: test if menu was clicked
                    if(game->state == MENU_STATE) {
                        switch (clickedButton(&e)) {
                            case 1:
                                resetGame(game, settings);
                                game->state = RUNNING_STATE;
                                break;

                            case 2:
                                game->state = QUIT_STATE;
                                break;

                            case 3:
                                if(settings->aiGame == 1) {
                                    settings->aiGame = 0;
                                } else {
                                    settings->aiGame = 1;
                                }
                                renderGrid(renderer);
                                renderMenu(renderer, settings);
                                SDL_RenderPresent(renderer);
                                break;

                            case 4:
                                if(settings->difficulty == EASY) {
                                    settings->difficulty = MEDIUM;
                                } else {
                                    settings->difficulty = EASY;
                                }
                                renderGrid(renderer);
                                renderMenu(renderer, settings);
                                SDL_RenderPresent(renderer);
                                break;
                            case 5:
                                if(settings->randomStart == 1) {
                                    settings->randomStart = 0;
                                } else {
                                    settings->randomStart = 1;
                                }
                                renderGrid(renderer);
                                renderMenu(renderer, settings);
                                SDL_RenderPresent(renderer);
                                break;
                        }
                    }
                    if(game->state != QUIT_STATE && game->state != MENU_STATE) {
                        clickedOnColumn(game, e.button.x / CELL_EDGE, e.button.y / CELL_EDGE, settings);
                        renderGame(renderer, game, settings);
                        SDL_RenderPresent(renderer);

                        if(settings->aiGame && game->aiTurn && game->state == RUNNING_STATE) {
                            computerTurn(game, settings);
                            sleep(1);
                            renderGame(renderer, game, settings);
                            SDL_RenderPresent(renderer);
                        }

                        /*
                        if(!game->state) { //TODO: why?
                            renderGame(renderer, game, settings);
                            SDL_RenderPresent(renderer);
                        }
                        */
                    }

                    break;
                /*
                case SDL_MOUSEMOTION:
                    break;
                    //TODO: fix motion animation
                    if(!(e.button.y / CELL_EDGE)) {
                        if(prev != (e.button.y / CELL_EDGE)) {
                            prev = renderHovering(renderer, e.button.x / CELL_EDGE, game);
                            renderGame(renderer, game, settings);
                            SDL_RenderPresent(renderer);
                        }
                    }
                */
                default: {
                }
            }
        }
    }


    //quit and deallocate everything
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    free(game);

    return EXIT_SUCCESS;
}
