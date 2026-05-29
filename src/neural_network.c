// Code for Player vs Neural Network 
// gcc game_logic.c init_game.c render.c gui.c -o neural_network.exe neural_network.c  -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

// #define SDL_MAIN_HANDLED

#include <stdio.h>
#include "gui/gui.h"
#include "game_logic/game_logic.h"
#include "init_game/init_game.h"
#include "constants/constants.h"
#include "render/render.h"
#include "window_manager/window_manager.h"
#include "menu/menu.h"




Window startNeuralNetworkWindow()
{
    NeuralNetwork nn;

    // Try loading saved weights
    if (load_weights(&nn)) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        SDL_Window * window = createWindow("Tic Tac Toe");
        SDL_Renderer*renderer =  SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED); 
        if(window == NULL || renderer == NULL)
            return 1;

        Game*game = initGame();
        game->mode=3;
        Score*score = calloc(1,sizeof(Score));
        initBackground("./assets/bg3.bmp",renderer);
        createBoardWith(renderer,game);
        showPlayerTurn(renderer,game,score);

        SDL_Rect exitBtn= {5, 5, 90,30};
        Button backBtn = {.button=exitBtn, .state=MAIN_MENU};
         SDL_Color color = {0, 174, 239,255};
        // createText(renderer,color,&exitBtn,"Return");
        // SDL_RenderPresent(renderer);

        SDL_Point mousePosition;
        SDL_Event windowEvent;

        SDL_Cursor *hand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND); // Use SDL_SYSTEM_CURSOR_ARROW, SDL_SYSTEM_CURSOR_CROSSHAIR, etc.
        SDL_Cursor *defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW); // Use SDL_SYSTEM_CURSOR_ARROW, SDL_SYSTEM_CURSOR_CROSSHAIR, etc.

    
        //running
        game->state = RUNNING;
        while(game->state != QUIT)
        {   
            int x, y;
            SDL_GetMouseState(&x,&y);
            SDL_PollEvent(&windowEvent);
            switch(windowEvent.type)
            {   
                // update when user clicks on the board.
                case SDL_MOUSEBUTTONDOWN:
                    mousePosition.x = windowEvent.motion.x;
                    mousePosition.y = windowEvent.motion.y;
                    if(isMouseHoverOver(mousePosition.x,mousePosition.y,&backBtn.button) && backBtn.state == MAIN_MENU)
                        game->state = QUIT;
                    updateBoardByPosition(mousePosition,game);
                    updateScore(game,score);
                    switchPlayer(game);
                    Draw(game,renderer);
                    showPlayerTurn(renderer,game,score);

                    
                    break;
                case SDL_QUIT:
                    game->state = QUIT;
                    break;
            }

            if(game->currentPlayer == NN && game->state ==  RUNNING)
            {   
                SDL_Delay(1000);
                printf("AI's turn\n");
                get_best_move(&nn, game);
                updateScore(game,score);
                switchPlayer(game);
                Draw(game,renderer);
                showPlayerTurn(renderer,game,score);
                
            }
            
            if(game->state == WIN || game->state == TIE)
            {   
                highlightWinRect(renderer,score,game);
                showWinScreen(game,score,renderer);
                resetBoard(renderer,game,score);    
                showPlayerTurn(renderer,game,score);
        
            }

            if(isMouseHoverOver(x,y,&backBtn.button))
                SDL_SetCursor(hand);
            else
                SDL_SetCursor(defaultCursor);
        

        }

        // clean up memory.
        free(game);
        free(score);
        SDL_FreeCursor(hand);
        SDL_FreeCursor(defaultCursor);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return MAIN_MENU;
    }
}