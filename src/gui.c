#include "gui/gui.h"

SDL_Window *createWindow(char*windowName)
{   
    // create SDL_windwo
     SDL_Window * window = SDL_CreateWindow(windowName,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_ALLOW_HIGHDPI);
     return window;
}
void createText(SDL_Renderer*renderer,SDL_Color color, SDL_Rect*rect,char*text)
{
    //load up font
    TTF_Font * font = TTF_OpenFont("./Font/MouseMemoirs-Regular.ttf",50);
    
    //load to CPU
    SDL_Surface* textSurface = TTF_RenderText_Solid(font,text,color);
    
    //load to GPU for rendering
    SDL_Texture*texturetext = SDL_CreateTextureFromSurface(renderer,textSurface);

    //copy to current rendering context
    SDL_RenderCopy(renderer,texturetext,NULL,rect);

    //free resources
    SDL_DestroyTexture(texturetext);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
   
    
}

// Button click detection function
int isMouseHoverOver(int x, int y, SDL_Rect* button) {
    return (x > button->x && x < button->x + button->w && y > button->y && y < button->y + button->h);
}
