

#include "gui/gui.h"
#include "constants/constants.h"
#include <stdio.h>
#include "window_manager/window_manager.h"
#include "menu/menu.h"



Window startMenu()
{
        // Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize TTF
    if (TTF_Init() < 0) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Window* window = createWindow("Main Menu");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // initBackground("bg2.bmp",renderer);

    // Define button rectangles
    Button buttons[5];
    createMenuButtons(buttons);

    SDL_RenderPresent(renderer);
    SDL_Point mousePosition;
    SDL_Event event;
    
    // call surface and texture here to lessen the load.
    // creating surface and texture in the loop place a heavy burden onto the computer as creating surface means loading up the CPU and texture means using the GPU.
    SDL_Surface *logoSurface =  SDL_LoadBMP("./assets/logo.bmp"); //logo
    SDL_Texture *logoTexture = SDL_CreateTextureFromSurface(renderer,logoSurface);
    SDL_Rect logoRect = {350,5,300,300};
    SDL_FreeSurface(logoSurface);

    SDL_Surface *bgSurface =  SDL_LoadBMP("./assets/bg2.bmp"); //menu bg
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(renderer,bgSurface);
    SDL_FreeSurface(bgSurface);

    

    int running = 1;
    while(running)
    {   
       
        
         SDL_RenderClear(renderer); // clear frame
         //initBackground("bg2.bmp",renderer);
        SDL_RenderCopy(renderer,bgTexture,NULL,NULL); //copy to currenbt render context for background
        SDL_RenderCopy(renderer,logoTexture,NULL,&logoRect); //copy to current render context for logo
        displayMenuButtons(buttons,renderer); //display text
        SDL_RenderPresent(renderer); //render them visually on to the screen
      

        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                running =0;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                mousePosition.x = event.motion.x;
                mousePosition.y = event.motion.y;
                int window_state =  retrieveWindowState(buttons,mousePosition,window,renderer); //retrieve window sttate upon clicking
                if(window_state != -1)
                {
                    SDL_DestroyWindow(window);
                    SDL_DestroyRenderer(renderer);
                    return window_state; // return window_state back to main so that it can navigated to other window through the switch statement 

                }
                break;

        }

       

    }
    //free memory
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(logoTexture);
    return EXIT;
}

void createMenuButtons(Button buttons[])
{   
    /*This function creates the button using the button  
    struct to associate the window state so that when 
    user click onto them it will navigate to that window*/
    
    SDL_Rect multiplayerBtn = {300, 325, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
    buttons[0].button = multiplayerBtn;
    buttons[0].state = MULTIPLAYER;

    SDL_Rect neuralnetworkBtn = {300, 385, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
    buttons[1].button = neuralnetworkBtn;
    buttons[1].state = NEURAL;

    SDL_Rect imperfectmmBtn = {300, 445, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
    buttons[2].button = imperfectmmBtn;
    buttons[2].state = IMPERFECT;

    SDL_Rect minimaxBtn = {300, 505, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT};
    buttons[3].button = minimaxBtn;
    buttons[3].state = MINIMAX;

    SDL_Rect exitBtn= {400, 565, 200,50};
    buttons[4].button = exitBtn;
    buttons[4].state = EXIT;
}

Window retrieveWindowState(Button buttons[], SDL_Point mousePosition, SDL_Window*window, SDL_Renderer*renderer)
{       
    /* this function checks if the user has clicked onto the button,
    if it is true, then return the window state associate with the button*/



        int x = mousePosition.x; // get the mouse click x position
        int y =mousePosition.y; // get mouse click y position
        for(int i = 0, n=5; i<n;i++)
        {   
           if(isMouseHoverOver(x,y,&buttons[0].button) && buttons[0].state == MULTIPLAYER)             //checks if mouse is within the boundary of the button. If it is click and within tbhe boundary then return back the window state.
                return MULTIPLAYER;
           else   if(isMouseHoverOver(x,y,&buttons[1].button) && buttons[1].state == NEURAL)
                return NEURAL;
           else   if(isMouseHoverOver(x,y,&buttons[2].button) && buttons[2].state == IMPERFECT)
                return IMPERFECT;
           else   if(isMouseHoverOver(x,y,&buttons[3].button) && buttons[3].state == MINIMAX)
                return MINIMAX;

           else  if(isMouseHoverOver(x,y,&buttons[4].button) && buttons[4].state == EXIT) 
                return EXIT;
           

                
        }

        return -1;
       
}
void displayMenuButtons(Button buttons[],SDL_Renderer*renderer)
{   
    
    /*This function is to define the colors when user hover on the text*/
    
    int mouseX, mouseY; //mouse position of x cooridinate and y coordinate
    SDL_GetMouseState(&mouseX, &mouseY);
    
     // original text color
    SDL_Color textColor = {255,108,92,255};
    SDL_Color textColor2 = {202, 254, 202,255};
    SDL_Color textColor3= {92, 255, 236,255};
    SDL_Color textColor4 = {255, 127, 127,255};
    SDL_Color textColor5 = {255,255,255,255};

    //original color to be changed when hovered
    SDL_Color hoverColor = { 204, 86, 74, 255 };
    SDL_Color hoverColor2 = {161, 196, 161,255};
    SDL_Color hoverColor3 = {73, 204, 188, 255};
    SDL_Color hoverColor4 = {204, 102, 102, 255};
    SDL_Color hoverColor5 = {200, 200, 200, 255};

    //set color when mouse hover on the button
    SDL_Color color1 = isMouseHoverOver(mouseX, mouseY, &buttons[0].button) ? hoverColor : textColor;
    SDL_Color color2 = isMouseHoverOver(mouseX, mouseY, &buttons[1].button) ? hoverColor2 : textColor2;
    SDL_Color color3 = isMouseHoverOver(mouseX, mouseY, &buttons[2].button) ? hoverColor3 : textColor3;
    SDL_Color color4 = isMouseHoverOver(mouseX, mouseY, &buttons[3].button) ? hoverColor4 : textColor4;
    SDL_Color color5 = isMouseHoverOver(mouseX, mouseY, &buttons[4].button) ? hoverColor5 : textColor5;

    // this is to display the text.
    createText(renderer, color1, &buttons[0].button, "Player vs Player");
    createText(renderer, color2, &buttons[1].button, "Player vs CPU (Easy)");
    createText(renderer, color3, &buttons[2].button, "Player vs CPU (Medium)");
    createText(renderer, color4, &buttons[3].button, "Player vs CPU (Impossible)");
    createText(renderer, color5, &buttons[4].button, "Exit");


}
