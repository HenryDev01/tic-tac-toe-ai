#include "../init_game/init_game.h"
#include "../SDL2/SDL.h"
#include "../constants/constants.h"


void Draw(Game*game, SDL_Renderer*renderer);
void drawCircleAt(int rectPosition, SDL_Renderer * renderer);
void drawCrossAt(int rectPosition,SDL_Renderer* renderer);
void drawCross(SDL_Rect*rect,SDL_Renderer*renderer);
void drawCircle(SDL_Rect*rect,SDL_Renderer*renderer);
void drawAll(SDL_Renderer*renderer, Game*game, SDL_Rect rects[]);