#include "render/render.h"
#include <stdio.h>
#include <math.h>

void Draw(Game*game, SDL_Renderer* renderer)
{   
    if(!game->playerHasMoved)
        return;
    
    if(game->board[game->lastMovePosition] == PLAYER_O)
        drawCircleAt(game->lastMovePosition,renderer); //draw circle when it is player o
    else if(game->board[game->lastMovePosition] == PLAYER_X)
        drawCrossAt(game->lastMovePosition,renderer); //draw when player X

    printf("\n");
}

void drawCross(SDL_Rect*rect,SDL_Renderer*renderer) //takes in the rect instead of position
{   
    //this function does not animate the cross
    int margin = 10; //set a margin of 10 so the cross does not touch edge of the rectangle.
    SDL_SetRenderDrawColor(renderer, 0,0, 255,128); //cross color
    // draw cross by defining the coordinates.
    SDL_RenderDrawLine(renderer,rect->x + margin,rect->y + margin, (rect->x + rect->w) - margin   , (rect->y + rect->h) - margin );
    SDL_RenderDrawLine(renderer, (rect->x + rect->w) - margin, rect->y + margin, rect->x + margin , (rect->y + rect->h) - margin);
    //SDL_RenderPresent(renderer);
}

void drawCircle(SDL_Rect*rect,SDL_Renderer*renderer) //takes in rect instead of position
{
    int c[2];
    int margin = 10;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
    SDL_SetRenderDrawColor(renderer, 128, 0, 0,128);
    int center_x = rect->x + (rect->w/2);
    int center_y = rect->y + (rect->h/2);
    int radius = (center_y - rect->y) - margin;
    
    const int precision = 250; 
    float step = 2 * M_PI / precision;

    // iterate from 0 to full circle, since  2pi represents a full circle. 
    // 0 =< theta <= 2*pi according to website. 
    for (float theta = 0; theta <= 2 * M_PI; theta += step) {
        int x = center_x + (int)(radius * cos(theta)); //  formula for x coordinate
        int y = center_y + (int)(radius * sin(theta)); // formula for y coordinate

        if(theta >  (2 * M_PI / precision))
            step = 2 * M_PI / 70;           //increase drawing speed after initial drawing precision
        
        if(theta >   (2 * M_PI / 70)*10)
              step = 2 * M_PI / 20;
        
        if(theta > 0)
            SDL_RenderDrawLine(renderer,c[0],c[1],x,y);
        else    
            SDL_RenderDrawLine(renderer,x,y,x,y);    

        c[0] = x;
        c[1] = y;



    }
    //SDL_RenderPresent(renderer);
}


void drawCrossAt(int rectPosition,SDL_Renderer* renderer) // draw circle at specific position. usually uses playerlastMoved
{   
    int margin = 20;    //margin of 20 to avoid cross touching the edges of rectangle
    SDL_Rect rect = rects[rectPosition]; 
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0,0, 255,128); //set color for rectangle

    //left diagonal
    int startingCoordinates[2] = {rect.x + margin, rect.y + margin};
    while(1)
    {   
 
        int destinationCoordinates[2] = {startingCoordinates[0] + 25, startingCoordinates[1] + 15};
        if(destinationCoordinates[0] >= rect.x + rect.w || destinationCoordinates[1] >= rect.y + rect.h)
            break;
        //draw the diagonal
        SDL_RenderDrawLine(renderer,startingCoordinates[0],startingCoordinates[1],destinationCoordinates[0],destinationCoordinates[1]);
        SDL_RenderPresent(renderer); //visually present it in a iteration so that the animation can be seen.
        SDL_Delay(25);
        //set the starting coordinates for the next iteration as the destination is always the starting coordinate for the next itartion
        startingCoordinates[0] = destinationCoordinates[0];
        startingCoordinates[1] = destinationCoordinates[1];
    }

    //right diagonal -similar left diagonal
       startingCoordinates[0] = rect.x + rect.w - margin;
       startingCoordinates[1] = rect.y + margin;
    while(1)
    {   
        int destinationCoordinates[2] = {startingCoordinates[0] - 25, startingCoordinates[1] + 15};
        if(destinationCoordinates[0] <= rect.x  || destinationCoordinates[1] >= rect.y + rect.h)
            break;
        SDL_RenderDrawLine(renderer,startingCoordinates[0],startingCoordinates[1],destinationCoordinates[0],destinationCoordinates[1]);
        SDL_RenderPresent(renderer);
        SDL_Delay(25);
        startingCoordinates[0] = destinationCoordinates[0];
        startingCoordinates[1] = destinationCoordinates[1];
    }
    
    


}


void drawCircleAt(int rectPosition, SDL_Renderer * renderer) //draw rect at specific position
{   
    // parametric algo

    int c[2];
    int margin = 10;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
    SDL_SetRenderDrawColor(renderer, 128, 0, 0,128);
    SDL_Rect rect = rects[rectPosition];
    int center_x = rect.x + (rect.w/2);
    int center_y = rect.y + (rect.h/2);
    int radius = (center_y - rect.y) - margin;
    
    const int precision = 250; 
    float step = 2 * M_PI / precision;

    // iterate from 0 to full circle, since  2pi represents a full circle. 
    // 0 =< theta <= 2*pi according to website. 
    for (float theta = 0; theta <= 2 * M_PI; theta += step) {
        int x = center_x + (int)(radius * cos(theta)); //  formula for x coordinate
        int y = center_y + (int)(radius * sin(theta)); // formula for y coordinate

        if(theta >  (2 * M_PI / precision))
            step = 2 * M_PI / 70;           //increase drawing speed after initial drawing precision
        
        if(theta >   (2 * M_PI / 70)*10)
              step = 2 * M_PI / 20;
        
        if(theta > 0)
            SDL_RenderDrawLine(renderer,c[0],c[1],x,y);
        else    
            SDL_RenderDrawLine(renderer,x,y,x,y);    

        c[0] = x;
        c[1] = y;

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

}
void drawAll(SDL_Renderer*renderer, Game*game,SDL_Rect rects[])
{   
    // this function is to draw all of crosses and rectangle 
    for (int i =0, n = DIMENSION*DIMENSION;i<n;i++)
    {   
        SDL_Rect rect = rects[i];
        if(game->board[i] == PLAYER_O)
            drawCircle(&rect,renderer);
        else if(game->board[i] == PLAYER_X) 
            drawCross(&rect,renderer);
    }
    
}
