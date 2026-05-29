#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "window_manager/window_manager.h"
#include "menu/menu.h"
#include "multiplayer/multiplayer.h"
#include "tic_tac_toe_imperfect_minimax/tic_tac_toe_imperfect_minimax.h"
#include "tic_tac_toe_minimax/tic_tac_toe_imperfect_minimax.h"
#include "neural_network/neural_network.h"
#include "gui/gui.h" 

int main(int argc, char* argv[]) {

    // main serve as a function to navigate to other window.

    SDL_Init(SDL_INIT_VIDEO);

    Window window = MAIN_MENU; // Start at the main menu
    
    SDL_Event event;

    while (window != EXIT) {
        SDL_PollEvent(&event);
        switch (window) {
            case MAIN_MENU:
                window = startMenu(); // Run the menu and capture the next state
                break;
            case MULTIPLAYER:
                window = startMultiplayerModeWindow(); // Run the game and capture the next state
                break;
            case MINIMAX:
                window = startMinimaxWindow();
                break;
            case IMPERFECT:
                window = startImperfectMinimaxWindow();
                break;
            case NEURAL:
                window = startNeuralNetworkWindow();
                break;
            case EXIT:
                break;
        }
    }

    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
