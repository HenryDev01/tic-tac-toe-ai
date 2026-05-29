
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

// Define an enumeration named 'Window' to represent various application states or screens.
// Each value corresponds to a specific state of the application, such as:
// - MAIN_MENU: The main menu screen
// - MULTIPLAYER: Multiplayer mode screen
// - MINIMAX: Perfect minimax AI mode
// - IMPERFECT: Imperfect AI mode
// - NEURAL: Neural network mode screen
// - EXIT: Exit the application
typedef enum {
    MAIN_MENU,
    MULTIPLAYER,
    MINIMAX,
    IMPERFECT,
    NEURAL,
    EXIT
} Window;

#endif
