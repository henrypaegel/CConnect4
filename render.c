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
const SDL_Color MENU_COLOR = {.r = 0, .g = 0, .b = 51, .a = 255};


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

void renderText(SDL_Renderer *renderer, int x, int y, char *textStr, char *fontStr, int ptSize, const SDL_Color *textColor) {
    TTF_Font *font = TTF_OpenFont(fontStr, ptSize);
    SDL_Surface *textSurface;

    textSurface = TTF_RenderUTF8_Solid(font, textStr, *textColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    SDL_Rect dstRect = { x-texW/2, y-texH/2, texW, texH };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
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

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(textSurface);

    TTF_CloseFont(font);
    free(bar);
} // renders whole background-color

int viewHighscore(char* text) {
    FILE *f = fopen("highscore.csv", "r");
    int moves, player;
    double time;
    fscanf(f, "%d,%lf,%d", &moves, &time, &player);
    sprintf(text, "%d moves in %.2lfs", moves, time);
    return player;
}

void renderMenu(SDL_Renderer *renderer, gameSettings *settings) {
    SDL_SetRenderDrawColor(renderer, MENU_COLOR.r, MENU_COLOR.g, MENU_COLOR.b, MENU_COLOR.a);
    SDL_Rect *menu;
    menu = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    menu->x = 2*CELL_EDGE;
    menu->y = 2*CELL_EDGE;
    menu->w = 3*CELL_EDGE;
    menu->h = 4.75*CELL_EDGE;
    SDL_RenderFillRect(renderer, menu);

    SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, 255);
    SDL_Rect *button;
    button = (SDL_Rect *) malloc(sizeof(SDL_Rect));

    int buttonBorder = 40;
    int spacing = 32;
    char* options[] = { "New Game",
                      "Exit Game" };

    button->x = 240;
    button->w = 3*CELL_EDGE - 2*buttonBorder;
    button->h = CELL_EDGE - buttonBorder;
    for (int i = 1; i < 3; ++i) {
        button->y = 2*CELL_EDGE + i*spacing + ((i-1) * (CELL_EDGE-buttonBorder));
        SDL_RenderDrawRect(renderer, button);
        renderText(renderer, button->x+button->w/2, button->y+button->h/2, options[i-1], "arial.ttf", 25, &WHITE);
    }

    button->y = 416;
    SDL_RenderDrawRect(renderer, button);
    if(settings->aiGame == 1) {
        renderText(renderer, button->x + button->w / 2, button->y + button->h / 2, "Player vs COM", "arial.ttf", 25,
                   &WHITE);
    } else if(settings->aiGame == 0){
        renderText(renderer, button->x+button->w/2, button->y+button->h/2, "Player vs Player", "arial.ttf", 25, &WHITE);
    }

    button->y = 492;
    button->w = 108;
    button->h = 60;
    SDL_RenderDrawRect(renderer, button);
    if(settings->difficulty == EASY) {
        renderText(renderer, button->x+button->w/2, button->y+button->h/2, "EASY", "arial.ttf", 20, &WHITE);
    } else if(settings->difficulty == MEDIUM) {
        renderText(renderer, button->x+button->w/2, button->y+button->h/2, "MEDIUM", "arial.ttf", 20, &WHITE);
    }

    button->x += 112;
    SDL_RenderDrawRect(renderer, button);
    if(settings->randomStart ==  1) {
        renderText(renderer, button->x+button->w/2, button->y+button->h/2, "RANDOM", "arial.ttf", 20, &WHITE);
    } else if(settings->randomStart == 0) {
        renderText(renderer, button->x+button->w/2, button->y+button->h/2, "PLAYER", "arial.ttf", 20, &WHITE);
    }

    char highscore[30];
    int player = viewHighscore(highscore);
    if(player == 1) {
        renderText(renderer, 350, 610, "Current Highscore:", "arial.ttf", 25, &WHITE);
        renderText(renderer, 350, 635, highscore, "arial.ttf", 25, &YELLOW_COLOR);
    } else if(player == 2) {
        renderText(renderer, 350, 610, "Current Highscore:", "arial.ttf", 25, &WHITE);
        renderText(renderer, 350, 635, highscore, "arial.ttf", 25, &RED_COLOR);
    }

    free(menu);
    free(button);
}

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

void renderGame(SDL_Renderer *renderer, const game_t *game, gameSettings *settings) {
    switch (game->state) {
        case RUNNING_STATE:
            renderBar(renderer, &WHITE, game);
            renderGrid(renderer);
            renderBoard(renderer, game);
            break;

        case RED_WON_STATE:
            renderGameOverState(renderer, game, &RED_COLOR);
            renderMenu(renderer, settings);
            break;

        case YELLOW_WON_STATE:
            renderGameOverState(renderer, game, &YELLOW_COLOR);
            renderMenu(renderer, settings);
            break;

        case TIE_STATE:
            renderGameOverState(renderer, game, &TIE_COLOR);
            renderMenu(renderer, settings);
            break;

        default: {
        }
    }
} // invoke different renderings with specific colors depending on game-state
