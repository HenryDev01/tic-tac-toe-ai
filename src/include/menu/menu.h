
#include "../SDL2/SDL.h"
#include "../window_manager/window_manager.h"
#include <stdbool.h>

// Define a struct to represent a button along with its associated state.
// The struct acts like a key-value pair, where the button (key) is represented
// by an SDL_Rect, and the corresponding state (value) is represented by a Window enum.
// This allows buttons to be linked to specific states for navigation purposes.
typedef struct {
    SDL_Rect button;
    Window state;
} Button;


Window startMenu();
//bool isMouseOverText(int mouseX, int mouseY, SDL_Rect textRect) ;
void createMenuButtons(Button buttons[]);
void displayMenuButtons(Button buttons[],SDL_Renderer*renderer);
Window retrieveWindowState(Button buttons[], SDL_Point mousePosition, SDL_Window*window, SDL_Renderer*renderer);
