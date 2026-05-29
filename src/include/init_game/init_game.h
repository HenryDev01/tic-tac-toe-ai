#include "../SDL2/SDL.h"
#include "../constants/constants.h"
#include <stdbool.h>



#ifndef GAME_H
#define GAME_H
typedef struct{ 
    int mode;
    int board[DIMENSION*DIMENSION];
    int totalMoveMade;
    bool playerHasMoved;
    int lastMovePosition; // tracking lastMovePosition ensure that iteration is not needed which makes the program faster.
    int state;
    int currentPlayer;
}Game;

#endif

#ifndef SCORE_H
#define SCORE_H
typedef struct{
 
    int rowsPoint[DIMENSION];
    int columnPoints[DIMENSION];
    int leftdiagonalPoint;
    int rightDiagonalPoint;
    int player_o_points;
    int player_x_points;
    int tie_points;
    int winner;
}Score;

#endif


#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK
typedef struct {
    double input[INPUT_NODES];
    double hidden[HIDDEN_NODES];
    double output[OUTPUT_NODES];
    double weights_input_hidden[INPUT_NODES][HIDDEN_NODES];
    double weights_hidden_output[HIDDEN_NODES][OUTPUT_NODES];
    double biases_hidden[HIDDEN_NODES];
    double bias_output[OUTPUT_NODES];
} NeuralNetwork;

#endif

#ifndef RECTS_H
#define RECTS_H
extern SDL_Rect rects[DIMENSION*DIMENSION];

#endif

Game*initGame();
void initBackground(char*imgName,SDL_Renderer*renderer);
void initPlayerScore(SDL_Renderer*renderer,Score*score,Game*game);
