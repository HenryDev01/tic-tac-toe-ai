#include "init_game/init_game.h"
#include <time.h>
#include <stdio.h>
#include "gui/gui.h"

SDL_Rect rects[DIMENSION*DIMENSION];

Game* initGame() //initialize the game struct
{   
    Game*game =  calloc(1,sizeof(Game)); //initialized all to zero using calloc to avoid garbage value.
    game->state = INITIALIZING;
    //set initial turns to random to ensure fairness.
    srand(time(0));
    int choice = (rand() % 2) * 2 - 1;  
    game->currentPlayer = choice;
    return game;
}

void initBackground(char*imgName,SDL_Renderer*renderer)
{   
    //load bmp image to CPU
    SDL_Surface*surface = SDL_LoadBMP(imgName);



    if(!surface)
    {
        printf("Failed to load BMP Image  %s", SDL_GetError());
        return;
    }
    //load image from CPU to GPU for rendering
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    
    if(!texture)
    {
        printf("Failed to create texture: %s", SDL_GetError());
        return;
    }
    //copy to the current rendering context
    SDL_RenderCopy(renderer,texture,NULL,NULL);

    //free memory
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void initPlayerScore(SDL_Renderer*renderer,Score*score,Game*game)
{   
    // this function display player score for player x and o

    char str[12];

    SDL_Color player_X_Color = {0,0,255,128}; //player x color
    SDL_Rect playerXRect = (struct SDL_Rect){.x=200,.y=0,.w=80,.h=70};
    if(game->mode==0) // game 0 means player vs player. 
        createText(renderer,player_X_Color,&playerXRect,"X"); //helper function
    else //player vs CPU
         createText(renderer,player_X_Color,&playerXRect,"YOU (X)"); //helper function
    
    // set color for player X points
    SDL_SetRenderDrawColor(renderer,0,0,255,128); 
    SDL_RenderDrawLine(renderer,197,65,270,65);
 
    //display points for player x
    SDL_Rect playerXPointsRect = (struct SDL_Rect){.x=200,.y=60,.w=70,.h=70};
    sprintf(str,"%i", score->player_x_points);
    createText(renderer,player_X_Color,&playerXPointsRect,str);

    //display points for player o or CPU
    SDL_Color player_O_Color = {128,0,0,128};
    SDL_Rect playerORect = (struct SDL_Rect){.x=700,.y=0,.w=70,.h=70};
    if(game->mode == 0)
        createText(renderer,player_O_Color,&playerORect,"O");
    else
        createText(renderer,player_O_Color,&playerORect,"AI (O)");


    SDL_SetRenderDrawColor(renderer,128,0,0,128);
    SDL_RenderDrawLine(renderer,700,65,767,65);

    SDL_Rect playerOPointsRect = (struct SDL_Rect){.x=700,.y=60,.w=70,.h=70};
    sprintf(str,"%i", score->player_o_points);
    createText(renderer,player_O_Color,&playerOPointsRect,str);

    //display points for tie.
    SDL_Color tie_color = {145, 78, 134,128};
    SDL_Rect tieRect = (struct SDL_Rect){.x=450,.y=0,.w=70,.h=70};
    createText(renderer,tie_color,&tieRect,"Tie");

    SDL_SetRenderDrawColor(renderer,145, 78, 134,128);
    SDL_RenderDrawLine(renderer,447,65,527,65);

    
    SDL_Rect tiePointsRect = (struct SDL_Rect){.x=450,.y=60,.w=70,.h=70};
    sprintf(str,"%i",score->tie_points );
    createText(renderer,tie_color,&tiePointsRect,str);

    //SDL_RenderPresent(renderer);

}
