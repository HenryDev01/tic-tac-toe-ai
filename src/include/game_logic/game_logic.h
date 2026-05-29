#include "../SDL2/SDL.h"
#include "../init_game/init_game.h"
#include <stdbool.h>
#include "../constants/constants.h"

//GUI functions and game-related functions
void createBoardWith(SDL_Renderer*renderer, Game*game);
void resetBoard(SDL_Renderer*renderer,Game*game,Score*score);
void switchPlayer(Game*game);
void showPlayerTurn(SDL_Renderer*renderer, Game*game, Score*score);
void showWinScreen(Game*game,Score*score,SDL_Renderer*renderer);
void updateScore(Game*game, Score*score);
void updatePlayerX(Game*game, Score*score);
void updatePlayerO(Game*game, Score*score);
void updateBoardByPosition(SDL_Point mousePosition, Game*game);
void highlightWinRect(SDL_Renderer*renderer,Score*score,Game*game);

//Minimax Algorithm
void computerMove(Game*game);
int minimax(int board[9], int player,int alphaScore, int betaScore, int depth);
void random_move();
int win(int board[9]);


//AI
int load_weights(NeuralNetwork *nn);
void forward_propagation(NeuralNetwork *nn, double inputs[INPUT_NODES]);
double sigmoid(double x);
double predict(NeuralNetwork *nn, double inputs[INPUT_NODES]);
int get_best_move(NeuralNetwork *nn, Game*game);
void encode_board(int board[9], double encoded_board[INPUT_NODES]);