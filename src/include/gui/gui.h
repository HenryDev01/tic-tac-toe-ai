#include "../SDL2/SDL.h"
#include "../SDL2/SDL_ttf.h"
#include "../constants/constants.h"

SDL_Window *createWindow(char*windowName);
void createText(SDL_Renderer*renderer,SDL_Color color, SDL_Rect*rect,char*text);
int isMouseHoverOver(int x, int y, SDL_Rect* button);
